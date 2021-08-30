#pragma once
#include "IOBase.hpp"
#include <QObject>
#include <QString>
#include <memory>
#include <vector>

class IOHandler : public QObject
{
	Q_OBJECT

	std::shared_ptr<IOBase> m_io;
	QString m_id;
	bool m_ready = false;
	bool m_error = false;
	QString m_errorMessage;

	std::vector<std::weak_ptr<IOHandler>> m_targets;

public:
	IOHandler(std::shared_ptr<IOBase> io)
		: QObject()
		, m_io(io)
	{
		QObject::connect(m_io.get(), &IOBase::ready, this, &IOHandler::onReady);
		QObject::connect(m_io.get(), &IOBase::errorOccured, this, &IOHandler::onErrorOccured);
		QObject::connect(m_io.get(), &IOBase::newData, this, &IOHandler::onNewData);
	}

	QString id() const
	{
		return m_id;
	}

	void setId(const QString& id)
	{
		m_id = id;
	}

	std::shared_ptr<IOBase> io() const
	{
		return m_io;
	}

	bool isReady() const
	{
		return m_ready;
	}

	bool isError() const
	{
		return m_error;
	}

	QString errorMessage() const
	{
		return m_errorMessage;
	}

	void addTarget(std::shared_ptr<IOHandler> ioh)
	{
		m_targets.push_back(ioh);
	}

private slots:
	void onReady()
	{
		m_ready = true;
	}

	void onErrorOccured(const QString& errorMessage)
	{
		m_error = true;
		m_errorMessage = errorMessage;
	}

	void onNewData(const QByteArray& data)
	{
		auto num = m_targets.size();
		for (auto i = 0; i < num; i++)
		{
			auto targetIO = m_targets.at(i).lock(); // TODO Good for performance?
			if (targetIO)
			{
				targetIO->io()->writeData(data);
			}
		}
	}
};