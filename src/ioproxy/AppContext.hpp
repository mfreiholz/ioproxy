#pragma once
#include "IOBase.hpp"
#include "IOHandler.hpp"
#include <QObject>
#include <memory>
#include <vector>


class AppContext
{
	std::vector<std::shared_ptr<IOHandler>> m_handlers;

public:
	std::vector<std::shared_ptr<IOHandler>>& handlers()
	{
		return m_handlers;
	}

	std::shared_ptr<IOHandler> findHandlerByID(const QString& id) const
	{
		auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [&](const std::shared_ptr<IOHandler>& h) {
			return id.compare(h->id()) == 0;
		});
		return it != m_handlers.end() ? *it : std::shared_ptr<IOHandler>();
	}

	bool add(std::shared_ptr<IOHandler> ioh)
	{
		m_handlers.push_back(ioh);
		return true;
	}

	bool isReady() const
	{
		auto num = m_handlers.size();
		for (int i = 0; i < num; i++)
		{
			if (!m_handlers[i]->isReady())
				return false;
		}
		return true;
	}
};