#pragma once
#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QVariant>
#include <QVector>

/// Config holds all required information to startup the Engine.
/**
	Different applications could provide different implementations for this class.
	Console applications may fill this by commandline parameters.
**/
class Config
{
public:
	struct IO
	{
		QString id;
		QString type;
		QMultiMap<QString, QVariant> parameters;
	};

	Config() = default;
	Config(Config&&) = default;
	virtual ~Config() = default;
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	const QVector<IO>& ios() const;
	const QVector<QPair<QString, QString>>& connections() const;

protected:
	QVector<IO> m_ios; ///< List of IO configs.
	QVector<QPair<QString, QString>> m_connections; ///< Connections between IOs (k=from, v=to).
};