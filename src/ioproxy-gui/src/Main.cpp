#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	a.setApplicationName("IOProxy GUI");
	a.setApplicationVersion("1.0.0");
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://insanefactory.com");
	a.setQuitOnLastWindowClosed(true);

	MainWindow mw;
	mw.setVisible(true);

	return a.exec();
}