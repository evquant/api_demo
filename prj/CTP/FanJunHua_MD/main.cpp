#include "mainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.Init();
	w.show();
	return a.exec();
}
