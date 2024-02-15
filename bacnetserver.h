#ifndef BACNETSERVER_H
#define BACNETSERVER_H

#include <QThread>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "bacnet/config.h"
#include "bacnet/bacdef.h"
#include "bacnet/bacdcode.h"
#include "bacnet/apdu.h"
#include "bacnet/dcc.h"
#include "bacnet/iam.h"
#include "bacnet/npdu.h"
#include "bacnet/getevent.h"
#include "bacnet/version.h"
#include "bacnet/basic/services.h"
#include "bacnet/datalink/dlenv.h"
#include "bacnet/basic/sys/filename.h"
#include "bacnet/basic/sys/mstimer.h"
#include "bacnet/basic/tsm/tsm.h"
#include "bacnet/basic/tsm/tsm.h"
#include "bacnet/datalink/datalink.h"
#include "bacnet/basic/binding/address.h"

/* include the device object */
#include "bacnet/basic/object/device.h"
/* objects that have tasks inside them */
#if (BACNET_PROTOCOL_REVISION >= 14)
#include "bacnet/basic/object/lo.h"
#endif
#if (BACNET_PROTOCOL_REVISION >= 24)
#include "bacnet/basic/object/color_object.h"
#include "bacnet/basic/object/color_temperature.h"
#endif
#include "bacnet/basic/object/lc.h"
#include "bacnet/basic/object/trendlog.h"
#if defined(INTRINSIC_REPORTING)
#include "bacnet/basic/object/nc.h"
#endif /* defined(INTRINSIC_REPORTING) */
#if defined(BACFILE)
#include "bacnet/basic/object/bacfile.h"
#endif /* defined(BACFILE) */
#if defined(BAC_UCI)
#include "bacnet/basic/ucix/ucix.h"
#endif /* defined(BAC_UCI) */

#include "bacnet/basic/object/ao.h"
#include "bacnet/basic/object/bo.h"

/** @file server/main.c  Example server application using the BACnet Stack. */

/* (Doxygen note: The next two lines pull all the following Javadoc
 *  into the ServerDemo module.) */
/** @addtogroup ServerDemo */
/*@{*/

/* current version of the BACnet stack */
static const char *BACnet_Version = BACNET_VERSION_TEXT;
/* task timer for various BACnet timeouts */
static struct mstimer BACnet_Task_Timer;
/* task timer for TSM timeouts */
static struct mstimer BACnet_TSM_Timer;
/* task timer for address binding timeouts */
static struct mstimer BACnet_Address_Timer;
#if defined(INTRINSIC_REPORTING)
/* task timer for notification recipient timeouts */
static struct mstimer BACnet_Notification_Timer;
#endif
/* task timer for objects */
static struct mstimer BACnet_Object_Timer;
/** Buffer used for receiving */
static uint8_t Rx_Buf[MAX_MPDU] = { 0 };

/** Initialize the handlers we will utilize.
 * @see Device_Init, apdu_set_unconfirmed_handler, apdu_set_confirmed_handler
 */
static void Init_Service_Handlers(void)
{
	Device_Init(NULL);
	/* we need to handle who-is to support dynamic device binding */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS,
				     handler_who_is);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_HAS,
				     handler_who_has);

#if 0
	/* 	BACnet Testing Observed Incident oi00107
		Server only devices should not indicate that they EXECUTE I-Am
		Revealed by BACnet Test Client v1.8.16 ( www.bac-test.com/bacnet-test-client-download )
BITS: BIT00040
Any discussions can be directed to edward@bac-test.com
Please feel free to remove this comment when my changes accepted after suitable time for
review by all interested parties. Say 6 months -> September 2016 */
	/* In this demo, we are the server only ( BACnet "B" device ) so we do not indicate
	   that we can execute the I-Am message */
	/* handle i-am to support binding to other devices */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, handler_i_am_bind);
#endif

	/* set the handler for all the services we don't implement */
	/* It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);
	/* Set the handlers for any confirmed services that we support. */
	/* We must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
				   handler_read_property);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
				   handler_read_property_multiple);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROPERTY,
				   handler_write_property);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE,
				   handler_write_property_multiple);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
				   handler_read_range);
#if defined(BACFILE)
	apdu_set_confirmed_handler(
				   SERVICE_CONFIRMED_ATOMIC_READ_FILE, handler_atomic_read_file);
	apdu_set_confirmed_handler(
				   SERVICE_CONFIRMED_ATOMIC_WRITE_FILE, handler_atomic_write_file);
#endif
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
				   handler_reinitialize_device);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
				     handler_timesync_utc);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
				     handler_timesync);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
				   handler_cov_subscribe);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
				     handler_ucov_notification);
	/* handle communication so we can shutup when asked */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
				   handler_device_communication_control);
	/* handle the data coming back from private requests */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_PRIVATE_TRANSFER,
				     handler_unconfirmed_private_transfer);

#if defined(INTRINSIC_REPORTING)
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ACKNOWLEDGE_ALARM,
				   handler_alarm_ack);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_GET_EVENT_INFORMATION,
				   handler_get_event_information);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_GET_ALARM_SUMMARY,
				   handler_get_alarm_summary);
#endif /* defined(INTRINSIC_REPORTING) */

#if defined(BACNET_TIME_MASTER)
	handler_timesync_init();
#endif

	apdu_set_confirmed_handler(SERVICE_CONFIRMED_CREATE_OBJECT,
				   handler_create_object);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DELETE_OBJECT,
				   handler_delete_object);
	/* configure the cyclic timers */
	mstimer_set(&BACnet_Task_Timer, 1000UL);
	mstimer_set(&BACnet_TSM_Timer, 50UL);
	mstimer_set(&BACnet_Address_Timer, 60UL*1000UL);
	mstimer_set(&BACnet_Object_Timer, 100UL);

#if defined(INTRINSIC_REPORTING)
	mstimer_set(&BACnet_Notification_Timer, NC_RESCAN_RECIPIENTS_SECS*1000UL);
#endif
}

class BacnetServer: public QThread
{
public:
	void run();
};

#endif
