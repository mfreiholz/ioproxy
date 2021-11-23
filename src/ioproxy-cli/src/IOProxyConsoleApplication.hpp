#pragma once
#include <QObject>
#include <QString>
#include <ioproxy/os/SignalHandler.hpp>
#include <memory>

namespace ioproxy
{
	class Engine;
	class ConsoleStatusPrinter;

	class IOProxyConsoleApplication : public QObject, public os::SignalHandler
	{
		Q_OBJECT

	public:
		IOProxyConsoleApplication(QObject* parent);
		~IOProxyConsoleApplication() override;

		bool handleSignal(os::Signal) override;

		Q_SLOT void startup();

	signals:
		void finished();
		void finishedWithSignal();
		void failed(int code, const QString& errorMessage);

	private:
		std::unique_ptr<Engine> m_engine;
		std::unique_ptr<ConsoleStatusPrinter> m_statusPrinter;
	};
}