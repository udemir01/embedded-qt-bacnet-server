#include "bacnetwidget.h"
#include "ui_bacnetwidget.h"

BacnetWidget::BacnetWidget(QWidget *parent)
: QWidget(parent)
	, ui(new Ui::BacnetWidget)
{
	ui->setupUi(this);
	this->show();

	BACNET_ADDRESS src = {}; /* address where message came from */
	uint16_t pdu_len = 0;
	unsigned timeout = 1; /* milliseconds */
	uint32_t elapsed_milliseconds = 0;
	uint32_t elapsed_seconds = 0;
	BACNET_CHARACTER_STRING DeviceName;

	int present_value;

#if defined(BACNET_TIME_MASTER)
	BACNET_DATE_TIME bdatetime;
#endif
#if defined(BAC_UCI)
	int uciId = 0;
	struct uci_context *ctx;
#endif

	Device_Set_Object_Instance_Number(666);

#if defined(BAC_UCI)
	ctx = ucix_init("bacnet_dev");
	if (!ctx)
		fprintf(stderr, "Failed to load config file bacnet_dev\n");
	uciId = ucix_get_option_int(ctx, "bacnet_dev", "0", "Id", 0);
	if (uciId != 0) {
		Device_Set_Object_Instance_Number(uciId);
	} else {
#endif /* defined(BAC_UCI) */
#if defined(BAC_UCI)
	}
	ucix_cleanup(ctx);
#endif /* defined(BAC_UCI) */

	printf("BACnet Server Demo\n"
	       "BACnet Stack Version %s\n"
	       "BACnet Device ID: %u\n"
	       "Max APDU: %d\n",
	       BACnet_Version, Device_Object_Instance_Number(), MAX_APDU);
	/* load any static address bindings to show up
	   in our device bindings list */
	address_init();
	Init_Service_Handlers();


#if defined(BAC_UCI)
	const char *uciname;
	ctx = ucix_init("bacnet_dev");
	if (!ctx)
		fprintf(stderr, "Failed to load config file bacnet_dev\n");
	uciname = ucix_get_option(ctx, "bacnet_dev", "0", "Name");
	if (uciname != 0) {
		Device_Object_Name_ANSI_Init(uciname);
	} else {
#endif /* defined(BAC_UCI) */
#if defined(BAC_UCI)
	}
	ucix_cleanup(ctx);
#endif /* defined(BAC_UCI) */

	if (Device_Object_Name(Device_Object_Instance_Number(), &DeviceName)) {
		printf("BACnet Device Name: %s\n", DeviceName.value);
	}

	dlenv_init();
	atexit(datalink_cleanup);
	/* broadcast an I-Am on startup */
	Send_I_Am(&Handler_Transmit_Buffer[0]);
	/* loop forever */
	for (;;) {
		/* input */
		pdu_len = datalink_receive(&src, &Rx_Buf[0], MAX_MPDU, timeout);

		/* process */
		if (pdu_len) {
			npdu_handler(&src, &Rx_Buf[0], pdu_len);
		}
		if (mstimer_expired(&BACnet_Task_Timer)) {
			mstimer_reset(&BACnet_Task_Timer);
			elapsed_milliseconds = mstimer_interval(&BACnet_Task_Timer);
			elapsed_seconds = elapsed_milliseconds/1000;
			/* 1 second tasks */
			dcc_timer_seconds(elapsed_seconds);
			datalink_maintenance_timer(elapsed_seconds);
			dlenv_maintenance_timer(elapsed_seconds);
			handler_cov_timer_seconds(elapsed_seconds);
			Load_Control_State_Machine_Handler();
			trend_log_timer(elapsed_seconds);

#if defined(INTRINSIC_REPORTING)
			Device_local_reporting();
#endif
#if defined(BACNET_TIME_MASTER)
			Device_getCurrentDateTime(&bdatetime);
			handler_timesync_task(&bdatetime);
#endif
		}
		if (mstimer_expired(&BACnet_TSM_Timer)) {
			mstimer_reset(&BACnet_TSM_Timer);
			elapsed_milliseconds = mstimer_interval(&BACnet_TSM_Timer);
			tsm_timer_milliseconds(elapsed_milliseconds);
		}
		if (mstimer_expired(&BACnet_Address_Timer)) {
			mstimer_reset(&BACnet_Address_Timer);
			elapsed_milliseconds = mstimer_interval(&BACnet_Address_Timer);
			elapsed_seconds = elapsed_milliseconds/1000;
			address_cache_timer(elapsed_seconds);
		}
		handler_cov_task();
#if defined(INTRINSIC_REPORTING)
		if (mstimer_expired(&BACnet_Notification_Timer)) {
			mstimer_reset(&BACnet_Notification_Timer);
			Notification_Class_find_recipient();
		}
#endif
		/* output */
		if (mstimer_expired(&BACnet_Object_Timer)) {
			mstimer_reset(&BACnet_Object_Timer);
			elapsed_milliseconds = mstimer_interval(&BACnet_Object_Timer);
			Device_Timer(elapsed_milliseconds);

			for (unsigned int i = 1;
			     i <= Analog_Output_Count();
			     i++){
				present_value = Analog_Output_Present_Value(i);
				printf("%d ", present_value);
			}
			printf("\n");
			for (unsigned int i = 1;
			     i <= Binary_Output_Count();
			     i++){
				present_value = Binary_Output_Present_Value(i);
				printf("%d ", present_value);
			}
			printf("\n");
		}
	}
}

BacnetWidget::~BacnetWidget()
{
	delete ui;
}
