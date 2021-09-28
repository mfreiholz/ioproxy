#pragma once
#include "AppContext.hpp"
#include "Global.hpp"
#include "os/SignalHandler.hpp"
#include <QObject>
#include <QTimer>
#include <memory>

class App
	: public QObject,
	  public os::SignalHandler
{
	Q_OBJECT

public:
	App(AppContext& context)
		: QObject(nullptr)
		, os::SignalHandler()
		, m_context(context)
		, m_statisticTimer()
	{
		auto& hh = m_context.handlers();
		for (int i = 0; i < hh.size(); ++i)
		{
			QObject::connect(hh[i]->io.get(), &IOBase::started, this, &App::onEntryStarted);
			QObject::connect(hh[i]->io.get(), &IOBase::startupErrorOccured, this, &App::onEntryStartupErrorOccured);
			QObject::connect(hh[i]->io.get(), &IOBase::errorOccured, this, &App::onEntryRuntimeErrorOccured);
		}
		QObject::connect(&m_statisticTimer, &QTimer::timeout, this, &App::onStatisticTimer);
	}

	~App() override
	{
	}

	bool handleSignal(os::Signal) override
	{
		HL_INFO(LL, "Quit on user request");
		emit signalQuit();
		return true;
	}

public slots:
	void startAll()
	{
		auto& hh = m_context.handlers();
		if (hh.empty())
		{
			HL_ERROR(LL, "Missing IO configuration. At least one IO is required.");
			emit abort();
			return;
		}

		for (int i = 0; i < hh.size() && !m_error; ++i)
		{
			hh[i]->io->start();
		}
		if (m_error)
		{
			HL_ERROR(LL, QString("Startup aborted.").toStdString());
			return;
		}
	}

	void stopAll()
	{
		auto& handlers = m_context.handlers();
		for (auto h : handlers)
		{
			h->io->stop();
		}
	}

private slots:
	void onEntryStarted()
	{
		m_readyCounter++;
		if (m_readyCounter != m_context.handlers().size())
			return;

		// all io's have been started!
		m_statisticTimer.setInterval(std::chrono::seconds(1));
		m_statisticTimer.start();

		HL_INFO(LL, QString("Startup OK: io_count=%1").arg(m_readyCounter, 3, 10, QChar('0')).toStdString());
		emit allStarted();
	}

	void onEntryStartupErrorOccured(const QString& errorString)
	{
		HL_ERROR(LL, errorString.toStdString());
		m_error = true;
		stopAll();
		emit abort();
	}

	void onEntryRuntimeErrorOccured(const QString& errorString)
	{
		HL_ERROR(LL, QString("onEntryRuntimeErrorOccured() -> %1").arg(errorString).toStdString());
	}

	void onStatisticTimer()
	{
		Statistic sum;
		for (const auto& h : m_context.handlers())
		{
			const auto& stats = h->io->statistic();
			sum.bytesRead += stats.bytesRead;
			sum.bytesWritten += stats.bytesWritten;
		}
		HL_INFO(LL, QString("Statistic: Read %1 bytes / Write %2 bytes").arg(sum.bytesRead).arg(sum.bytesWritten).toStdString());
	}

signals:
	void abort();
	void allStarted();
	void signalQuit();

private:
	AppContext& m_context;
	int m_readyCounter = 0;
	bool m_error = false;

	QTimer m_statisticTimer;
};