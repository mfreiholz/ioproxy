#include <ioproxy/Config.hpp>
using namespace ioproxy;

const QVector<Config::IO>& Config::ios() const
{
	return m_ios;
}

const QMultiMap<QString, QString>& Config::connections() const
{
	return m_connections;
}