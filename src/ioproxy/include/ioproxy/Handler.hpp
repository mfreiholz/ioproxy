#pragma once
#include <QObject>
#include <ioproxy/IOBase.hpp>
#include <memory>

namespace ioproxy
{
	class Engine;

	/// Handler
	/**
		Handler manages the lifecycle and communication with the IOBase object.
		The handler may even move the event-processing of IOBase into a different thread.
		That's why communication with IOBase should only happen via signal/slots.
	**/
	class Handler : public QObject
	{
		Q_OBJECT

	public:
		Handler(Engine* engine, std::unique_ptr<IOBase> io);
		Handler(Handler&&) = default;
		~Handler() override;
		Handler(const Handler&) = delete;
		Handler& operator=(const Handler&) = delete;

		Q_SLOT void start();
		Q_SLOT void stop();

	signals:
		void started();
		void finished();

	private:
		Q_SLOT void onStarted();
		Q_SLOT void onStartupErrorOccured(const QString& errorMessage);
		Q_SLOT void onErrorOccured(const QString& errorMessage);

		Engine* m_engine = nullptr;
		std::unique_ptr<IOBase> m_io;

		friend class Engine;
	};
}