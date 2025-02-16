#pragma once
#include "io/IOBase.hpp"
#include <QObject>
#include <memory>

namespace ioproxy
{
	/// Handler
	/**
		Handler manages the lifecycle and communication with the IOBase object.
		The handler may even move the event-processing of IOBase into a different thread.
		That's why communication with IOBase should only happen via signal/slots.
	**/
	class Handler : public QObject
	{
		Q_OBJECT
		Q_PROPERTY(QString uniqueName READ uniqueName CONSTANT)
		Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
		Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)

	public:
		enum State
		{
			Stopped,
			Started,
			Failed,
		};
		Q_ENUM(State)

		Handler(std::shared_ptr<IOBase> io, QObject* parent);
		~Handler() override;

		QString uniqueName() const;

		State state() const;
		void setState(State state);

		QString errorMessage() const;
		void setErrorMessage(const QString& message);

		qint64 bytesWritten() const;
		qint64 bytesRead() const;

	public Q_SLOTS:
		void start();
		void stop();
		void writeData(const DataPack& data);

	private Q_SLOTS:
		void onStarted();
		void onStartupErrorOccured(const QString& errorMessage);
		void onErrorOccured(const QString& errorMessage);
		void onNewData(const DataPack& data);

	Q_SIGNALS:
		void stateChanged();
		void errorMessageChanged();
		void newData(const DataPack& data);

	private:
		std::shared_ptr<IOBase> m_io;
		State m_state = State::Stopped;
		QString m_errorMessage;

		qint64 m_bytesWritten = 0;
		qint64 m_bytesRead = 0;
	};
}