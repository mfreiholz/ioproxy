#include "ConsoleStatusPrinter.hpp"
#include <QDateTime>
#include <chrono>
#include <humblelogging/humblelogging.h>
#include <ioproxy/Engine.hpp>
#include <ioproxy/HumanReadableFormatter.hpp>
using namespace ioproxy;

HUMBLE_LOGGER(L, "ioproxy-cli.status");

ConsoleStatusPrinter::ConsoleStatusPrinter(Engine* engine, QObject* parent)
	: QObject(parent)
	, m_engine(engine)
	, m_statisticTimer()
{
	QObject::connect(&m_statisticTimer, &QTimer::timeout, this, &ConsoleStatusPrinter::onStatisticTimer);
}

ConsoleStatusPrinter::~ConsoleStatusPrinter()
{
	stop();
}

void ConsoleStatusPrinter::start()
{
	m_statisticTimer.setInterval(std::chrono::seconds(1));
	m_statisticTimer.start();
}

void ConsoleStatusPrinter::stop()
{
	m_statisticTimer.stop();
}

void ConsoleStatusPrinter::onStatisticTimer()
{
	// Sum up newest values.
	auto nowTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
	Statistic sum = m_engine->statisticSummary();

	// Compare with previous statistics and calculate data rates.
	auto bytesReadDiff = sum.bytesRead - m_previousStatistic.bytesRead;
	auto bytesWriteDiff = sum.bytesWritten - m_previousStatistic.bytesWritten;
	auto timestampDiff = nowTimestamp - m_previousStatisticTimestamp;

	qint64 bytesReadPerSecond = ((double)bytesReadDiff / (double)timestampDiff) * 1000.0;
	qint64 bytesWritePerSecond = ((double)bytesWriteDiff / (double)timestampDiff) * 1000.0;

	m_previousStatisticTimestamp = nowTimestamp;
	m_previousStatistic = sum;
	HL_INFO(L, QString("Statistic: Read %1 (%2/s) / Write %3 (%4/s)")
				   .arg(HumanReadableFormatter::fileSize(sum.bytesRead))
				   .arg(HumanReadableFormatter::fileSize(bytesReadPerSecond))
				   .arg(HumanReadableFormatter::fileSize(sum.bytesWritten))
				   .arg(HumanReadableFormatter::fileSize(bytesWritePerSecond))
				   .toStdString());
}