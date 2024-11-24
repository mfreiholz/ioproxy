#pragma once
#include "Config.hpp"
#include <tl/expected.hpp>

class CmdLineConfig : public Config
{
public:
	CmdLineConfig();
	~CmdLineConfig() override = default;
	CmdLineConfig(const CmdLineConfig&) = delete;
	CmdLineConfig& operator=(const CmdLineConfig&) = delete;

	tl::expected<void, QString> fromArguments(const QStringList& arguments);
	QString usage() const;

private:
	bool hasNext() const;
	bool next(QString& value);
	bool peekNext(QString& value);

	QStringList m_arguments;
	int m_index = -1;
};