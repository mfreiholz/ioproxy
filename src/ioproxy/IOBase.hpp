#pragma once
#include <QByteArray>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

class IOBase : public QObject
{
	Q_OBJECT

public:
	IOBase(QObject* parent = nullptr)
		: QObject(parent)
	{}

	~IOBase() override = default;

public slots:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void writeData(const QByteArray& data) { Q_UNUSED(data); }

signals:
	void ready();
	void newData(const QByteArray& data);
	void errorOccured(const QString& errorMessage);
};