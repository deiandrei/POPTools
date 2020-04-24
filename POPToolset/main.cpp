#include "POPToolset.h"
#include <QtWidgets/QApplication>
#include <Windows.h>

int main(int argc, char *argv[]) {
#ifdef _WIN32
	if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
#endif

	QApplication a(argc, argv);
	POPToolset w;
	w.show();
	return a.exec();
}
