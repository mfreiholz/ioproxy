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
		Statistic statisticSummary() const;

	public Q_SLOTS:
		void start();
		void stop();

	private:
		friend class Handler;

		QMap<QString, IOFactoryBase*> m_ioFactories;
		QList<std::shared_ptr<Handler>> m_handlers;

		bool m_started = false;

		tl::expected<std::shared_ptr<Handler>, QString> getIOHandler(const QString& id) const;
	};
}