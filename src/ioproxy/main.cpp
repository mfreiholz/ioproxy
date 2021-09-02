#include "App.hpp"
#include "AppContext.hpp"
#include "CommandLineInitializer.hpp"
#include <QSerialPortInfo>
#include <QtCore/QCoreApplication>
#include <humblelogging/humblelogging.h>
#include <iostream>

void initLogging()
{
	using namespace humble::logging;
	Factory& fac = Factory::getInstance();
	fac.setDefaultFormatter(new PatternFormatter("%date\t%lls\t%m\n"));
	fac.setConfiguration(new SimpleConfiguration(LogLevel::All));
	fac.registerAppender(new ConsoleAppender());
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
	initLogging();

	if (a.arguments().size() >= 2 && a.arguments()[1] == "serialports")
	{
		printSerialPorts();
		return 0;
	}

	AppContext context;

	CommandLineInitializer cmdInit(a.arguments());
	if (!cmdInit.init(context))
		return -1;

	App app(context);
	QObject::connect(&app, &App::abort, &a, &QCoreApplication::quit);
	QMetaObject::invokeMethod(&app, &App::startAll, Qt::QueuedConnection);

	return a.exec();
}