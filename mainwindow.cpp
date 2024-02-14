#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	BacnetServer *server = new BacnetServer();
	server->start();

	QTimer *timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(5000);

}

void MainWindow::update()
{
	ui->analog_output_field_1->setText(QString::number(Analog_Output_Present_Value(1)));
	ui->analog_output_field_2->setText(QString::number(Analog_Output_Present_Value(2)));
	ui->analog_output_field_3->setText(QString::number(Analog_Output_Present_Value(3)));
	ui->analog_output_field_4->setText(QString::number(Analog_Output_Present_Value(4)));
	ui->binary_output_field_1->setText(QString::number(Binary_Output_Present_Value(1)));
	ui->binary_output_field_2->setText(QString::number(Binary_Output_Present_Value(2)));
	ui->binary_output_field_3->setText(QString::number(Binary_Output_Present_Value(3)));
	ui->binary_output_field_4->setText(QString::number(Binary_Output_Present_Value(4)));
}

MainWindow::~MainWindow()
{
	delete ui;
}
