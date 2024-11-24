#pragma once
#include "io/IOBase.hpp"
#include <QObject>
#include <memory>

namespace ioproxy
{
	class Engine;

	/// Handler
	/**
		Handler manages the lifecycle and communication with the IOBase object.
		The handler may even move the event-processing of IOBase into a different thread.
		That's why communication with IOBase should only happen via signal/slots.
	**/
	class Handler : public QObject
	{
		Q_OBJECT
		Q_PROPERTY(State state READ getState NOTIFY stateChanged)
		Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)

	public:
		enum State
		{
			Stopped,
			Started,
			Failed,
		};
		Q_ENUM(State)

		Handler(Engine* engine, std::shared_ptr<IOBase> io);
		~Handler() override;

		std::shared_ptr<IOBase> io() const;

		State getState() const;
		void setState(State state);

		QString getErrorMessage() const;
		void setErrorMessage(const QString& message);

	public Q_SLOTS:
		void start();
		void stop();

	private Q_SLOTS:
		void onStarted();
		void onStartupErrorOccured(const QString& errorMessage);
		void onErrorOccured(const QString& errorMessage);

	Q_SIGNALS:
		void stateChanged();
		void errorMessageChanged();

	private:
		friend class Engine;

		Engine* m_engine = nullptr;
		std::shared_ptr<IOBase> m_io;

		State m_state = State::Stopped;
		QString m_errorMessage;
	};
}