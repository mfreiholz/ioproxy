#pragma once
#include "IOBase.hpp"
#include <QObject>
#include <QString>
#include <QThread>
#include <memory>
#include <vector>

class IOHandler : public QObject
{
	Q_OBJECT

public:
	explicit IOHandler(std::shared_ptr<IOBase> io);
	~IOHandler() override;

	IOHandler(const IOHandler&) = delete;
	IOHandler& operator=(const IOHandler&) = delete;

	std::shared_ptr<IOBase> io() const;

	void addTarget(std::shared_ptr<IOHandler> ioh)
	{
		m_targets.push_back(ioh);
	}

private slots:
	void onNewData(const DataPack& data)
	{
		auto num = m_targets.size();
		for (auto i = 0; i < num; i++)
		{
			auto targetIO = m_targets.at(i).lock(); // TODO Good for performance?
			if (targetIO)
			{
				targetIO->m_io->writeData(data);
			}
		}
	}

private:
	std::shared_ptr<IOBase> m_io;
	std::vector<std::weak_ptr<IOHandler>> m_targets;
};