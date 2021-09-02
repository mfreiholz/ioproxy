#pragma once
#include "IOBase.hpp"
#include <QObject>
#include <QString>
#include <memory>
#include <vector>

class IOHandler : public QObject
{
	Q_OBJECT

public:
	std::shared_ptr<IOBase> io;

	IOHandler(std::shared_ptr<IOBase> io_)
		: QObject()
		, io(io_)
	{
		QObject::connect(io.get(), &IOBase::newData, this, &IOHandler::onNewData);
	}

	~IOHandler() override
	{}

	void addTarget(std::shared_ptr<IOHandler> ioh)
	{
		m_targets.push_back(ioh);
	}

private slots:
	void onNewData(const QByteArray& data)
	{
		auto num = m_targets.size();
		for (auto i = 0; i < num; i++)
		{
			auto targetIO = m_targets.at(i).lock(); // TODO Good for performance?
			if (targetIO)
			{
				targetIO->io->writeData(data);
			}
		}
	}

private:
	std::vector<std::weak_ptr<IOHandler>> m_targets;
};