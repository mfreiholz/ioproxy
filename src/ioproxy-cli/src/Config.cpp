#include "Config.hpp"

const QVector<Config::IO>& Config::ios() const
{
	return m_ios;
}

const QVector<QPair<QString, QString>>& Config::connections() const
{
	return m_connections;
}