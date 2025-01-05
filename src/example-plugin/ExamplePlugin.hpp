#pragma once
#include <QObject>
#include "ioproxy/PluginAPI.hpp"

class ExamplePlugin : public QObject, public PluginAPI
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "com.insanefactory.ioproxy.exampleplugin/1.0")
	Q_INTERFACES(PluginAPI)

public:
	ExamplePlugin();
	~ExamplePlugin() override;
	QString id() const override;
	QString description() const override;
	QString author() const override;
	QList<IOFactoryBase*> ioFactories() const override;

private:
	QList<IOFactoryBase*> m_factories;
};