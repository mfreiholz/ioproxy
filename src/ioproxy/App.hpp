#pragma once
#include "AppContext.hpp"
#include "Global.hpp"
#include "os/WindowsSignalHandler.hpp"
#include <QObject>
#include <memory>

class App : public QObject, public WindowsSignalHandler
{
	Q_OBJECT

public:
	App(AppContext& context)
		: QObject(nullptr)
		, WindowsSignalHandler(Signals::Defaults)
		, m_context(context)
	{
		auto& hh = m_context.handlers();
		for (int i = 0; i < hh.size(); ++i)
		{
			QObject::connect(hh[i]->io.get(), &IOBase::started, this, &App::onEntryStarted);
			QObject::connect(hh[i]->io.get(), &IOBase::startupErrorOccured, this, &App::onEntryStartupErrorOccured);
			QObject::connect(hh[i]->io.get(), &IOBase::errorOccured, this, &App::onEntryRuntimeErrorOccured);
		}
	}

	~App() override
	{
	}

	bool handleSignal(Signals) override
	{
		HL_INFO(LL, "Quit on user request");
		emit signalQuit();
		return true;
	}

public slots:
	void startAll()
	{
		auto& hh = m_context.handlers();
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

signals:
	void abort();
	void allStarted();
	void signalQuit();

private:
	AppContext& m_context;
	int m_readyCounter = 0;
	bool m_error = false;
};