#pragma once
#include "IOBase.hpp"
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

	void writeData(const QByteArray& data) override
	{
		QString str(data);
		auto s = str.toStdString();
		std::cout << s;
	}

private:
	Options m_options;
};