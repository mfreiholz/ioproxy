#pragma once
#include "Global.hpp"
#include "Handler.hpp"
#include "io/IOBase.hpp"
#include "io/IOFactoryBase.hpp"
#include <QObject>
#include <memory>
#include <tl/expected.hpp>

namespace ioproxy
{
	class Engine : public QObject
	{
		Q_OBJECT

	public:
		static void RegisterMetaTypes();

		Engine(QObject* parent = nullptr);
		~Engine() override;

		tl::expected<void, QString> registerBuiltIn();

		tl::expected<void, QString> registerIOFactory(IOFactoryBase* ioFactory);
		tl::expected<IOFactoryBase*, QString> findFactoryByID(const QString& factoryID) const;

		tl::expected<void, QString> addIO(std::shared_ptr<IOBase> io);
		tl::expected<void, QString> addConnection(const QString& srcIO, const QString& dstIO);

		const QMap<QString, IOFactoryBase*>& ioFactories() const { return m_ioFactories; }
		const QList<Handler*>& handlers() const { return m_handlers; }

		Statistic statisticSummary() const;

	public Q_SLOTS:
		void start();
		void stop();

	private:
		friend class Handler;

		// All known IO factories.
		QMap<QString, IOFactoryBase*> m_ioFactories;

		// List of Handlers which manage the IOs added by "addIO()".
		QList<Handler*> m_handlers;

		// If "true", the initial call to "start()" has already been made.
		bool m_started = false;

		tl::expected<Handler*, QString> getIOHandler(const QString& id) const;
	};
}