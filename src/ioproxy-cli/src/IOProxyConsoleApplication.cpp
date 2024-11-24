#include "IOProxyConsoleApplication.hpp"
#include "CmdLineConfig.hpp"
#include "ConsoleStatusPrinter.hpp"
#include "ioproxy/Engine.hpp"

IOProxyConsoleApplication::IOProxyConsoleApplication(const QStringList& arguments, QObject* parent)
	: QObject(parent)
	, os::SignalHandler()
	, m_arguments(arguments)
{}

bool IOProxyConsoleApplication::handleSignal(os::Signal)
{
	Q_EMIT quitted();
	return true;
}

void IOProxyConsoleApplication::start()
{
	CmdLineConfig config;
	auto ok = config.fromArguments(m_arguments);
	if (!ok)
	{
		Q_EMIT errorOccurred(ok.error());
		return;
	}


	// Setup engine with IOs from config.
	m_engine = new ioproxy::Engine(this);
	ok = m_engine->registerBuiltIn();
	if (!ok)
	{
		Q_EMIT errorOccurred(ok.error());
		return;
	}


	// Register IOs from "CmdLineConfig".
	for (const Config::IO& configIO : config.ios())
	{
		auto eFactory = m_engine->findFactoryByID(configIO.type);
		if (!eFactory)
		{
			Q_EMIT errorOccurred(eFactory.error());
			return;
		}

		auto eIO = eFactory.value()->create(configIO.parameters);
		if (!eIO)
		{
			Q_EMIT errorOccurred(eIO.error());
			return;
		}
		eIO.value()->setUniqueName(configIO.id);

		auto eAdded = m_engine->addIO(std::shared_ptr<IOBase>(eIO.value()));
		if (!eAdded)
		{
			Q_EMIT errorOccurred(eAdded.error());
			return;
		}
	}


	// Setup connections between IOs.
	const auto cc = config.connections();
	for (auto it = cc.cbegin(); it != cc.cend(); ++it)
	{
		auto ok = m_engine->addConnection(it->first, it->second);
		if (!ok)
		{
			Q_EMIT errorOccurred(ok.error());
			return;
		}
	}


	// Start all IOs.
	m_engine->start();


	// Start logging of statistics to STDOUT.
	m_statusPrinter = new ConsoleStatusPrinter(m_engine, this);
	m_statusPrinter->start();
}