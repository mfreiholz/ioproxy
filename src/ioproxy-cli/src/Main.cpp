#include "CmdLineConfig.hpp"
#include "IOProxyConsoleApplication.hpp"
#include <QtCore/QCoreApplication>
#include <ioproxy/Engine.hpp>
#include <ioproxy/Logging.hpp>
#include <ioproxy/Version.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

HUMBLE_LOGGER(L, "ioproxy-cli");

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("IOProxy CLI");
	a.setApplicationVersion(IOPROXY_VERSION);
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://www.insanefactory.com");

	ioproxy::PrintAppInfoHeader();

	ioproxy::InitializeLogging();
	ioproxy::Engine::RegisterMetaTypes();

	ioproxy::IOProxyConsoleApplication ioApp(nullptr);
	QObject::connect(&ioApp, &ioproxy::IOProxyConsoleApplication::finished, &a, &QCoreApplication::quit);
	QObject::connect(&ioApp, &ioproxy::IOProxyConsoleApplication::finishedWithSignal, &a, [&]() {
		HL_INFO(L, "Quit on user request.");
		a.quit();
	});
	QObject::connect(&ioApp, &ioproxy::IOProxyConsoleApplication::failed, [&](int code, const QString& errorMessage) {
		HL_INFO(L, QString("Quit on error (%1) %2").arg(QString::number(code)).arg(errorMessage).toStdString());
		a.quit();
	});
	QMetaObject::invokeMethod(&ioApp, &ioproxy::IOProxyConsoleApplication::startup, Qt::QueuedConnection);

	return a.exec();
}