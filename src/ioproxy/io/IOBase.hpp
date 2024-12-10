#pragma once
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QVariant>
#include <cinttypes>

/*!
 * \brief The DataPack class
 */
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

	DataPack(const QByteArray& data)
		: bytes(data)
	{}

	~DataPack() = default;

	QByteArray bytes;
	int fixedSize = 0;
	QList<int64_t> timestamps;
	QMap<QString, QVariant> parameters;
};
Q_DECLARE_METATYPE(DataPack);


/*!
 * \brief The Statistic class
 */
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
	Q_PROPERTY(QString uniqueName READ uniqueName CONSTANT)

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
		if (uniqueName != m_uniqueName)
		{
			m_uniqueName = uniqueName;
			Q_EMIT uniqueNameChanged();
		}
	}

	const Statistic& statistic() const
	{
		return m_statistic;
	}

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void writeData(const DataPack&) {}

Q_SIGNALS:
	void errorOccured(const QString& errorMessage);
	void uniqueNameChanged();

	void startupErrorOccured(const QString& errorString);
	void started();

	void newData(const DataPack& data);

protected:
	QString m_uniqueName;
	bool m_useRawData = true;
	Statistic m_statistic;
};