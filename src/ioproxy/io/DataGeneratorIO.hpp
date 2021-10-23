#pragma once
#include "../IOBase.hpp"
#include <QTimer>

class DataGeneratorIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		int packetSize = 508;
		int maxBytesPerSecond = 1024 * 1024 * 5; // 5 MB/s
	};

	DataGeneratorIO();
	~DataGeneratorIO() override;

	void setOptions(Options&& options);
	void start() override;
	void stop() override;

private slots:
	void onTimer();

private:
	Options m_options;
	QTimer m_timer;

	int m_packetCountOnTimer = 1;
};