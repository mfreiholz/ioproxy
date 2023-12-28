#include <QApplication>
#include "Widgets/IOProxyMainWindow.hpp"

int main(int argc, char** argv)
{
	QApplication qapp(argc, argv);
	qapp.setQuitOnLastWindowClosed(true);

	IOProxyMainWindow win(nullptr);
	win.show();

	return qapp.exec();
}