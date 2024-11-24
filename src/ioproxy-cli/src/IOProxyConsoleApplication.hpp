#pragma once
#include "ConsoleStatusPrinter.hpp"
#include "ioproxy/os/SignalHandler.hpp"
#include <QObject>
#include <QString>
#include <memory>

namespace ioproxy
{
	class Engine;
}

class IOProxyConsoleApplication : public QObject, public os::SignalHandler
{
	Q_OBJECT

public:
	IOProxyConsoleApplication(const QStringList& arguments, QObject* parent = nullptr);
	bool handleSignal(os::Signal) override;

public Q_SLOTS:
	void start();

Q_SIGNALS:
	void errorOccurred(const QString& reason);
	void quitted();

private:
	QStringList m_arguments;
	ioproxy::Engine* m_engine = nullptr;
	ConsoleStatusPrinter* m_statusPrinter = nullptr;
};