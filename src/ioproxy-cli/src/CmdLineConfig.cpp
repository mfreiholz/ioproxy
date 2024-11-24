#include "CmdLineConfig.hpp"
#include <iostream>
#include <memory>
//#include <Qt>

CmdLineConfig::CmdLineConfig()
	: Config()
{}

tl::expected<void, QString> CmdLineConfig::fromArguments(const QStringList& arguments)
{
	m_arguments = arguments;
	if (m_arguments.isEmpty())
		return tl::make_unexpected(QString("Empty argument list."));

	QString val;
	std::unique_ptr<Config::IO> ioConfig;

	auto add2config = [&]() {
		if (ioConfig)
		{
			m_ios.append(*ioConfig);
			ioConfig.reset();
		}
	};

	while (next(val))
	{
		if (val == "-io")
		{
			add2config();
			if (!next(val))
			{
				continue;
			}
			ioConfig = std::make_unique<Config::IO>();
			ioConfig->type = val;
		}
		else if ((val == "-name" || val == "-id") && ioConfig)
		{
			if (!next(val))
			{
				continue;
			}
			ioConfig->id = val;
		}
		else if (val == "-p" && ioConfig)
		{
			if (!next(val))
			{
				continue;
			}
			auto pos = val.indexOf("=");
			if (pos == -1)
			{
				ioConfig->parameters.insert(val, QString("true"));
			}
			else
			{
				ioConfig->parameters.insert(val.mid(0, pos), val.mid(pos + 1));
			}
		}
		else if (val == "-connect")
		{
			if (!next(val))
			{
				QString reason = QString("Missing value for -connect <source>,<destination>");
				return tl::make_unexpected(reason);
			}
			const QStringList kv = val.split(',', Qt::SkipEmptyParts);
			if (kv.size() != 2)
			{
				QString reason("Invalid syntax for -connection argument");
				return tl::make_unexpected(reason);
			}
			m_connections.push_back(qMakePair(kv[0].trimmed(), kv[1].trimmed()));
		}
	}
	add2config();
	return {};
}

QString CmdLineConfig::usage() const
{
	return {};
}

bool CmdLineConfig::hasNext() const
{
	return m_arguments.size() > (m_index + 1);
}

bool CmdLineConfig::next(QString& value)
{
	if (!hasNext())
	{
		value = QString();
		return false;
	}
	value = m_arguments[++m_index];
	return true;
}

bool CmdLineConfig::peekNext(QString& value)
{
	if (!hasNext())
	{
		value = QString();
		return false;
	}
	value = m_arguments[m_index + 1];
	return true;
}