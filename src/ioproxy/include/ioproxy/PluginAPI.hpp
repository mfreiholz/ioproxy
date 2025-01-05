#pragma once
#include <QtPlugin>
#include <QList>
#include "../../io/IOFactoryBase.hpp"
#define IOProxy_PluginAPI_iid "com.insanefactory.ioproxy.prxPluginApi_iid/1.0"

class PluginAPI
{
public:
	virtual QString id() const = 0;
	virtual QString description() const = 0;
	virtual QString author() const = 0;
	virtual QList<IOFactoryBase*> ioFactories() const = 0;
};

Q_DECLARE_INTERFACE(PluginAPI, IOProxy_PluginAPI_iid)