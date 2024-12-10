#include "Engine.hpp"
#include <QHostAddress>
#include <QMetaType>

#include "io/SerialPortIO.hpp"
#include "io/StdOutIO.hpp"
#include "io/TcpServerIO.hpp"
#include "io/TcpSocketIO.hpp"
#include "io/UdpSocketIO.hpp"

namespace ioproxy
{
	void Engine::RegisterMetaTypes()
	{
		qRegisterMetaType<DataPack>("DataPack");
		qRegisterMetaType<QHostAddress>("QHostAddress");
	}

	Engine::Engine(QObject* parent)
		: QObject(parent)
	{}

	Engine::~Engine()
	{
		qDeleteAll(m_ioFactories);
	}

	tl::expected<void, QString> Engine::registerBuiltIn()
	{
		tl::expected<void, QString> fail;
		IOFactoryBase* ioFactory = nullptr;

		ioFactory = new SerialPortIOFactory();
		if (!(fail = registerIOFactory(ioFactory)))
			return fail;

		ioFactory = new StdOutFactory();
		if (!(fail = registerIOFactory(ioFactory)))
			return fail;

		ioFactory = new TcpSocketFactory();
		if (!(fail = registerIOFactory(ioFactory)))
			return fail;

		ioFactory = new TcpServerIOFactory();
		if (!(fail = registerIOFactory(ioFactory)))
			return fail;

		ioFactory = new UdpSocketIOFactory();
		if (!(fail = registerIOFactory(ioFactory)))
			return fail;

		return fail;
	}

	tl::expected<void, QString> Engine::registerIOFactory(IOFactoryBase* ioFactory)
	{
		if (!ioFactory)
		{
			return tl::make_unexpected("NULL parameter");
		}
		if (m_ioFactories.contains(ioFactory->getID()))
		{
			return tl::make_unexpected(QString("Factory \"%1\" already registered.").arg(ioFactory->getID()));
		}
		m_ioFactories.insert(ioFactory->getID(), ioFactory);
		return {};
	}

	tl::expected<IOFactoryBase*, QString> Engine::findFactoryByID(const QString& factoryID) const
	{
		auto it = m_ioFactories.find(factoryID);
		if (it == m_ioFactories.cend())
			return tl::make_unexpected(QString("Factory with ID \"%1\" not registered.").arg(factoryID));
		return it.value();
	}

	tl::expected<void, QString> Engine::addIO(std::shared_ptr<IOBase> io)
	{
		Handler* handler = new Handler(this, io);
		if (m_started)
		{
			handler->start();
		}
		m_handlers.push_back(handler);
		return {};
	}

	tl::expected<void, QString> ioproxy::Engine::addConnection(const QString& srcIO, const QString& dstIO)
	{
		auto eSrc = getIOHandler(srcIO);
		if (!eSrc)
			return tl::make_unexpected(eSrc.error());

		auto eDst = getIOHandler(dstIO);
		if (!eDst)
			return tl::make_unexpected(eDst.error());

		auto src = eSrc.value();
		auto dst = eDst.value();
		QObject::connect(src->io().get(), &IOBase::newData, dst->io().get(), &IOBase::writeData);
		return {};
	}

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
		if (m_started)
			return;
		for (const auto& handler : m_handlers)
		{
			handler->start();
		}
		m_started = true;
	}

	void Engine::stop()
	{
		for (const auto& handler : m_handlers)
		{
			handler->stop();
		}
		m_started = false;
	}

	tl::expected<Handler*, QString> Engine::getIOHandler(const QString& id) const
	{
		for (auto it = m_handlers.cbegin(); it != m_handlers.cend(); it++)
		{
			if ((*it)->io()->uniqueName().compare(id, Qt::CaseInsensitive) == 0)
			{
				return *it;
			}
		}
		return tl::make_unexpected(QString("Can't find IO (id=%1)").arg(id));
	}
}