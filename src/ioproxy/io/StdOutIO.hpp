#pragma once
#include "../IOBase.hpp"
#include <iostream>

class StdOutIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
	};

	StdOutIO()
		: IOBase()
	{}

	~StdOutIO() override
	{
		stop();
	}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start() override
	{
		emit started();
	}

	void stop() override
	{
	}

	void writeData(const DataPack& data) override
	{
		QString str(data.bytes);
		auto s = str.toStdString();
		std::cout << s;

		m_statistic.bytesWritten += str.size();
	}

private:
	Options m_options;
};