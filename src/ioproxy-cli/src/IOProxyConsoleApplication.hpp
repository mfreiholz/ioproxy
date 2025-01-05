#pragma once
#include "ConsoleStatusPrinter.hpp"
#include "ioproxy/os/SignalHandler.hpp"
#include <QObject>
#include <QString>
#include <memory>
#include <QPluginLoader>
#include <QList>
#include "ioproxy/PluginAPI.hpp"

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
	void printCommandLineUsage() const;

public Q_SLOTS:
	void start();

Q_SIGNALS:
	void errorOccurred(const QString& reason);
	void quitted();

private:
	QStringList m_arguments;
	ioproxy::Engine* m_engine = nullptr;
	ConsoleStatusPrinter* m_statusPrinter = nullptr;

	QList<QPluginLoader*> m_pluginLoaders;
	QList<PluginAPI*> m_pluginInstances;
};