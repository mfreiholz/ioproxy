#include "IOProxyConsoleApplication.hpp"
#include "CmdLineConfig.hpp"
#include "ConsoleStatusPrinter.hpp"
#include <QtCore/QCoreApplication>
#include <ioproxy/Engine.hpp>
using namespace ioproxy;

IOProxyConsoleApplication::IOProxyConsoleApplication(QObject* parent)
	: QObject(parent)
	, os::SignalHandler()
{}

IOProxyConsoleApplication::~IOProxyConsoleApplication()
{}

bool IOProxyConsoleApplication::handleSignal(os::Signal)
{
	emit finishedWithSignal();
	return true;
}

void IOProxyConsoleApplication::startup()
{
	CmdLineConfig config = {};
	if (!config.fromArguments(QCoreApplication::instance()->arguments()))
	{
		return;
	}

	//Engine::RegisterMetaTypes();
	m_engine = std::make_unique<Engine>(std::move(config), this);
	m_engine->start();

	m_statusPrinter = std::make_unique<ConsoleStatusPrinter>(m_engine.get(), this);
	m_statusPrinter->start();
}