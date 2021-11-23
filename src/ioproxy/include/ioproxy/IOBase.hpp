#pragma once
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QVariant>
#include <cinttypes>

class DataPack
{
public:
	DataPack()
	{}

	DataPack(const DataPack& other)
	{
		bytes = other.bytes;
		fixedSize = other.fixedSize;
		timestamps = other.timestamps;
		parameters = other.parameters;
	}

	DataPack(const QByteArray& data_)
		: bytes(data_)
	{}

	~DataPack() = default;

	QByteArray bytes;
	int fixedSize = 0;
	QList<int64_t> timestamps;
	QMap<QString, QVariant> parameters;
};
Q_DECLARE_METATYPE(DataPack);

struct Statistic
{
	int64_t bytesRead = 0;
	int64_t bytesWritten = 0;
};

/*!
	Base object for all types that can input/output data.
	It should always be possible to create an empty stateless object from the specific implementation.
	It is fine to pass options in constructor, but the fun part have to start in "start()" method and not before!
*/
class IOBase : public QObject
{
	Q_OBJECT

public:
	IOBase()
		: QObject(nullptr)
	{}

	~IOBase() override = default;

	QString uniqueName() const
	{
		return m_uniqueName;
	}

	void setUniqueName(const QString& uniqueName)
	{
		Q_ASSERT(!uniqueName.isEmpty());
		if (uniqueName != m_uniqueName)
		{
			m_uniqueName = uniqueName;
		}
	}

	const Statistic& statistic() const
	{
		return m_statistic;
	}

public slots:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void writeData(const DataPack&) {}

signals:
	void started(); ///< Emitted if start was successful
	void startupErrorOccured(const QString& errorString); ///< Emitted if start failed

	void newData(const DataPack& data);
	void errorOccured(const QString& errorMessage);

protected:
	QString m_uniqueName;
	Statistic m_statistic;
};