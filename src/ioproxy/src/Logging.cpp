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
		fac.setDefaultFormatter(std::make_unique<PatternFormatter>("%date\t%lls\t%m\n"));
		fac.setConfiguration(std::make_unique<Configuration>(LogLevel::All));
		fac.registerAppender(std::make_shared<ConsoleAppender>());
	}

	void PrintAppInfoHeader()
	{
		auto qapp = QCoreApplication::instance();
		std::cout << qapp->applicationName().toStdString() << " " << qapp->applicationVersion().toStdString() << std::endl;
	}
}