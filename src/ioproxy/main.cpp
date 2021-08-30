#include "App.hpp"
#include "AppContext.hpp"
#include "CommandLineInitializer.hpp"
#include <QtCore/QCoreApplication>
#include <humblelogging/humblelogging.h>

void initLogging()
{
	using namespace humble::logging;
	Factory& fac = Factory::getInstance();
	fac.setConfiguration(new SimpleConfiguration(LogLevel::All));
	fac.registerAppender(new ConsoleAppender());
}

/*
	Command line arguments idea.
	--io text --id text1 --params text="hello world"&interval_ms=500
	--io stdout --id stdout1
	--io udpsocket --id udp1 --params bind_address=127.0.0.1&bind_port=13371&remote_address=127.0.0.1&remote_port=13372
	--io udpsocket --id udp2 --params bind_address=127.0.0.1&bind_port=13372&remote_address=127.0.0.1&remote_port=13371
	--connect=text1,udp1
	--connect=text1,udp2
	--connect=udp1,stdout1
	--connect=udp2,stdout1
*/
int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	initLogging();

	AppContext context;

	CommandLineInitializer cmdInit(a.arguments());
	if (!cmdInit.init(context))
		return -1;


	App app(context);
	app.startAll();
	return a.exec();
}
