#include "srutils.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SRUtils w;
	w.show();
	return a.exec();
}
