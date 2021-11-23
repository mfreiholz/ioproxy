#include <QRandomGenerator>
#include <chrono>
#include <ioproxy/io/DataGeneratorIO.hpp>

DataGeneratorIO::DataGeneratorIO()
	: IOBase()
{}

DataGeneratorIO::~DataGeneratorIO() = default;

void DataGeneratorIO::setOptions(Options&& options)
{
	m_options = options;
}

void DataGeneratorIO::start()
{
	// How many packets do we need to send per second to reach the requested amount.
	const double packetsPerSecond = static_cast<double>(m_options.maxBytesPerSecond) / static_cast<double>(m_options.packetSize);

	// We can not sleep for more than X ms.
	const double lowestSleepTime = 10;
	double sleepTime = 1000.0 / packetsPerSecond;
	m_packetCountOnTimer = sleepTime < lowestSleepTime ? packetsPerSecond / (1000.0 / lowestSleepTime) : 1;
	sleepTime = sleepTime < lowestSleepTime ? lowestSleepTime : sleepTime;

	// Setup timer stuff.
	QObject::connect(&m_timer, &QTimer::timeout, this, &DataGeneratorIO::onTimer);
	m_timer.setInterval(std::chrono::milliseconds(static_cast<int>(sleepTime)));
	m_timer.start();
	emit started();
}

void DataGeneratorIO::stop()
{
	m_timer.disconnect(this);
	m_timer.stop();
}

void DataGeneratorIO::onTimer()
{
	for (auto i = 0; i < m_packetCountOnTimer; i++)
	{
		QByteArray bytes;
		bytes.resize(m_options.packetSize);
		m_statistic.bytesWritten += bytes.size();
		emit newData(bytes);
	}
}