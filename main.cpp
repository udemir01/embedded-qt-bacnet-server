#include <QApplication>

#include "bacnetwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BacnetWidget w;
	w.show();
	return a.exec();
}
