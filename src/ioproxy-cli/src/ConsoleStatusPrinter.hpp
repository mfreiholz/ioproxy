#pragma once
#include <QObject>
#include <QTimer>
#include <ioproxy/IOBase.hpp>

namespace ioproxy
{
	class Engine;

	class ConsoleStatusPrinter : public QObject
	{
		Q_OBJECT

	public:
		ConsoleStatusPrinter(Engine* engine, QObject* parent);
		~ConsoleStatusPrinter() override;

		Q_SLOT void start();
		Q_SLOT void stop();

	private:
		Q_SLOT void onStatisticTimer();

		Engine* m_engine = nullptr;

		QTimer m_statisticTimer;
		Statistic m_previousStatistic;
		qint64 m_previousStatisticTimestamp = 0;
	};
}