#pragma once
#include <ioproxy/Config.hpp>

namespace ioproxy
{
	/// CmdLineConfig initializes a `Config` object from command line arguments.
	/**
	**/
	class CmdLineConfig : public Config
	{
	public:
		CmdLineConfig();
		~CmdLineConfig() override;
		CmdLineConfig(const CmdLineConfig&) = delete;
		CmdLineConfig& operator=(const CmdLineConfig&) = delete;

		bool fromArguments(QStringList arguments);
		QString usage() const;

	private:
		bool hasNext() const;
		bool next(QString& value);
		bool peekNext(QString& value);

		QStringList m_arguments;
		int m_index = -1;
	};
}