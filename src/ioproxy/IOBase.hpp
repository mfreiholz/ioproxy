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
	bool m_useRawData = true;
	Statistic m_statistic;
};

class IOParameterDefinition
{
public:
	enum DataType
	{
		INT8,
		INT16,
		INT32,
		INT64,
		STRING,
	};

	QString name;
	DataType type = DataType::STRING;
	bool required = false;
	bool multi = false;
};


using IOParameters = QMultiMap<QString, QVariant>;


class IOFactory
{
public:
	virtual QString getID() const = 0;
	virtual QList<IOParameterDefinition> getParameterDefinitions() const = 0;
	virtual IOBase* create(const IOParameters& parameters);
};
