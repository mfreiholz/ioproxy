#include "EchoPlugin.hpp"
#include "EchoIO.hpp"

EchoPlugin::EchoPlugin()
	: QObject(nullptr)
	, PluginAPI()
{
	m_factories.append(new EchoIOFactory());
}

EchoPlugin::~EchoPlugin()
{}

QString EchoPlugin::id() const
{
	return QString("EchoPlugin");
}

QString EchoPlugin::description() const
{
	return QString("Acts as example.");
}

QString EchoPlugin::author() const
{
	return QString("Manuel Freiholz");
}

QList<IOFactoryBase*> EchoPlugin::ioFactories() const
{
	return m_factories;
}