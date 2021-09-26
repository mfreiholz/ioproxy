#include "App.hpp"
#include "AppContext.hpp"
#include "CommandLineInitializer.hpp"
#include "IOBase.hpp"
#include <QMetaType>
#include <QSerialPortInfo>
#include <QtCore/QCoreApplication>
#include <humblelogging/humblelogging.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

HUMBLE_LOGGER(L, "main");

void initConsole()
{
#ifdef WIN32
	AllocConsole();
#endif
}

void initLogging()
{
	using namespace humble::logging;
	Factory& fac = Factory::getInstance();
	fac.setDefaultFormatter(new PatternFormatter("%date\t%lls\t%m\n"));
	fac.setConfiguration(new SimpleConfiguration(LogLevel::All));
	fac.registerAppender(new ConsoleAppender());
}

void initMetaTypes()
{
	qRegisterMetaType<DataPack>("DataPack");
}

void printSerialPorts()
{
	auto portInfos = QSerialPortInfo::availablePorts();
	for (auto portInfo : portInfos)
	{
		std::cout << portInfo.portName().toStdString() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("IOProxy");
	a.setApplicationVersion("1.0.0");
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://insanefactory.com");

	//initConsole();
	initLogging();
	initMetaTypes();

	HL_INFO(L, QString("%1 - %2").arg(a.applicationName()).arg(a.applicationVersion()).toStdString());

	if (a.arguments().size() >= 2 && a.arguments()[1] == "serialports")
	{
		printSerialPorts();
		return 0;
	}

	AppContext context;
	CommandLineInitializer cmdInit(a.arguments());
	if (!cmdInit.init(context))
	{
		HL_ERROR(L, "Error during command line initialization.");
		return -1;
	}

	App app(context);
	QObject::connect(&app, &App::abort, &a, &QCoreApplication::quit);
	QObject::connect(&app, &App::signalQuit, &a, &QCoreApplication::quit);
	QMetaObject::invokeMethod(&app, &App::startAll, Qt::QueuedConnection);

	return a.exec();
}