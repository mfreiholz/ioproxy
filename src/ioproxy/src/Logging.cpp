#include <QtCore/QCoreApplication>
#include <ioproxy/Logging.hpp>
#include <ioproxy/Version.hpp>
#include <iostream>

namespace ioproxy
{
	void InitializeLogging()
	{
		using namespace humble::logging;
		Factory& fac = Factory::getInstance();
		fac.setDefaultFormatter(new PatternFormatter("%date\t%lls\t%m\n"));
		fac.setConfiguration(new SimpleConfiguration(LogLevel::All));
		fac.registerAppender(new ConsoleAppender());
	}

	void PrintAppInfoHeader()
	{
		auto qapp = QCoreApplication::instance();
		std::cout << qapp->applicationName().toStdString() << " " << qapp->applicationVersion().toStdString() << std::endl;
	}
}