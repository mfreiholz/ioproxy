#pragma once
#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QVector>

namespace ioproxy
{
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
			QMap<QString, QString> parameters;
		};

		Config() = default;
		Config(Config&&) = default;
		virtual ~Config() = default;
		Config(const Config&) = delete;
		Config& operator=(const Config&) = delete;

		const QVector<IO>& ios() const;
		const QMultiMap<QString, QString>& connections() const;

	protected:
		QVector<IO> m_ios; ///< List of IO configs.
		QMultiMap<QString, QString> m_connections; ///< Connections between IOs (k=from, v=to).
	};
}