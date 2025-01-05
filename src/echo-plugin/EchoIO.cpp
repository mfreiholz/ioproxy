#include "EchoIO.hpp"

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

EchoIO::EchoIO()
	: IOBase()
{}

EchoIO::~EchoIO()
{}

void EchoIO::start()
{
	Q_EMIT started();
}

void EchoIO::stop()
{}

void EchoIO::writeData(const DataPack& data)
{
	Q_EMIT newData(data);
}

///////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////

EchoIOFactory::EchoIOFactory()
	: IOFactoryBase()
{}

QString EchoIOFactory::getID() const
{
	return EchoIOFactory::ID;
}

QList<IOParaDef> EchoIOFactory::getParameterDefinitions() const
{
	return m_paraDefs;
}

tl::expected<IOBase*, QString> EchoIOFactory::create(const IOParameters& parameters)
{
	return {new EchoIO()};
}