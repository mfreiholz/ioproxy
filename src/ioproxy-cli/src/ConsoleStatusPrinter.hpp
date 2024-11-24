#pragma once
#include "ioproxy/io/IOBase.hpp"
#include <QObject>
#include <QTimer>

namespace ioproxy
{
	class Engine;
}

class ConsoleStatusPrinter : public QObject
{
	Q_OBJECT

public:
	ConsoleStatusPrinter(ioproxy::Engine* engine, QObject* parent);
	~ConsoleStatusPrinter() override;

public Q_SLOTS:
	void start();
	void stop();
	void printStatistics();

private:
	ioproxy::Engine* m_engine = nullptr;

	QTimer m_statisticTimer;
	Statistic m_previousStatistic;
	qint64 m_previousStatisticTimestamp = 0;
};