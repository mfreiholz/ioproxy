#include <QHostAddress>
#include <QMetaType>
#include <humblelogging/humblelogging.h>
#include <ioproxy/Engine.hpp>
#include <ioproxy/Factory.hpp>
using namespace ioproxy;

HUMBLE_LOGGER(L, "ioproxy.engine");

void Engine::RegisterMetaTypes()
{
	qRegisterMetaType<DataPack>("DataPack");
	qRegisterMetaType<QHostAddress>("QHostAddress");
}

Engine::Engine(Config config, QObject* parent)
	: QObject(parent)
	, m_config{std::move(config)}
{
	for (const Config::IO& ioConfig : m_config.ios())
	{
		auto io = Factory::createIO(ioConfig);
		if (!io)
		{
			HL_ERROR(L, QString("Can't create IO: %1").arg(ioConfig.type).toStdString());
			continue;
		}
		auto handler = std::make_unique<Handler>(this, std::move(io));
		m_handlers.push_back(std::move(handler));
	}
}

Engine::~Engine()
{}

Statistic Engine::statisticSummary() const
{
	Statistic sum;
	for (const auto& h : m_handlers)
	{
		const auto& stats = h->m_io->statistic();
		sum.bytesRead += stats.bytesRead;
		sum.bytesWritten += stats.bytesWritten;
	}
	return sum;
}

void Engine::start()
{
	for (const auto& handler : m_handlers)
	{
		handler->start();
	}
}

void Engine::stop()
{
	for (const auto& handler : m_handlers)
	{
		handler->stop();
	}
}