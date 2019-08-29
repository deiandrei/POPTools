#include "POPToolset.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	POPToolset w;
	w.show();
	return a.exec();
}
