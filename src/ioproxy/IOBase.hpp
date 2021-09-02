#pragma once
#include <QByteArray>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

/*!
	Base object for all types that can input/output data.
	It should always be possible to create an empty stateless object from the specific implementation.
	It is fine to pass options in constructor, but the fun part have to start in "start()" method and not before!
*/
class IOBase : public QObject
{
	Q_OBJECT

public:
	IOBase(QObject* parent = nullptr)
		: QObject(parent)
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

	QString errorString() const
	{
		return m_errorString;
	}

public slots:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void writeData(const QByteArray&) {}

signals:
	void started(); ///< Emitted if start was successful
	void startupErrorOccured(const QString& errorString); ///< Emitted if start failed

	void newData(const QByteArray& data);
	void errorOccured(const QString& errorMessage);

protected:
	QString m_uniqueName;
	QString m_errorString;
};