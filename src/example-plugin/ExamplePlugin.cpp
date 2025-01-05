#include "ExamplePlugin.hpp"
#include "EchoIO.hpp"

ExamplePlugin::ExamplePlugin()
	: QObject(nullptr)
	, PluginAPI()
{
	m_factories.append(new EchoIOFactory());
}

ExamplePlugin::~ExamplePlugin()
{}

QString ExamplePlugin::id() const
{
	return QString("ExamplePlugin");
}

QString ExamplePlugin::description() const
{
	return QString("Acts as example.");
}

QString ExamplePlugin::author() const
{
	return QString("Manuel Freiholz");
}

QList<IOFactoryBase*> ExamplePlugin::ioFactories() const
{
	return m_factories;
}