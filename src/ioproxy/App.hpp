#pragma once
#include "AppContext.hpp"
#include <QObject>
#include <memory>

class App : public QObject
{
	Q_OBJECT
	AppContext& m_context;

public:
	explicit App(AppContext& context)
		: QObject(nullptr)
		, m_context(context)
	{}

	void startAll()
	{
		auto& handlers = m_context.handlers();
		for (auto h : handlers)
		{
			h->io()->start();
		}
	}

	void stopAll()
	{
		auto& handlers = m_context.handlers();
		for (auto h : handlers)
		{
			h->io()->stop();
		}
	}
};