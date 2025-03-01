#include "CmdLineConfig.hpp"
#include "IOProxyConsoleApplication.hpp"
#include "ioproxy/Engine.hpp"
#include "ioproxy/Version.hpp"
#include <QtCore/QCoreApplication>
#include <humblelogging/humblelogging.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

HUMBLE_LOGGER(LL, "ioproxy");

void initLogging()
{
	using namespace humble::logging;
	Factory& fac = Factory::getInstance();
	fac.setDefaultFormatter(std::make_unique<PatternFormatter>("%date\t%lls\t%m\n"));
	fac.setConfiguration(std::make_unique<Configuration>(LogLevel::All));
	fac.registerAppender(std::make_shared<ConsoleAppender>());
}

void printAppInfoHeader()
{
	auto qapp = QCoreApplication::instance();
	std::cout << qapp->applicationName().toStdString() << " " << qapp->applicationVersion().toStdString() << std::endl;
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("IOProxy CLI");
	a.setApplicationVersion(IOPROXY_VERSION);
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://www.insanefactory.com");

	initLogging();
	printAppInfoHeader();
	ioproxy::Engine::RegisterMetaTypes();

	IOProxyConsoleApplication ioApp(a.arguments(), nullptr);

	QObject::connect(&ioApp, &IOProxyConsoleApplication::errorOccurred, [&](const QString& reason) {
		HL_ERROR(LL, QString("Error: %1").arg(reason).toStdString());
		a.exit(1);
	});

	QObject::connect(&ioApp, &IOProxyConsoleApplication::quitted, [&]() {
		HL_ERROR(LL, QString("Quit").toStdString());
		a.quit();
	});

	QMetaObject::invokeMethod(&ioApp, &IOProxyConsoleApplication::start, Qt::QueuedConnection);
	return a.exec();
}