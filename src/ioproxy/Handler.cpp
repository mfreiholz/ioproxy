#include "Handler.hpp"
#include <humblelogging/humblelogging.h>

HUMBLE_LOGGER(LL, "ioproxy");

namespace ioproxy
{
	Handler::Handler(Engine* engine, std::shared_ptr<IOBase> io)
		: m_engine(engine)
		, m_io(io)
	{
		// @todo If it should be possible to run IO in a separate thread,
		// we need to move it to the thread right here, bevore we connect
		// any signal/slots.
	}

	Handler::~Handler()
	{
		stop();
	}

	std::shared_ptr<IOBase> Handler::io() const
	{
		return m_io;
	}

	Handler::State ioproxy::Handler::getState() const
	{
		return m_state;
	}

	void Handler::setState(State state)
	{
		if (m_state != state)
		{
			m_state = state;
			Q_EMIT stateChanged();
		}
	}

	QString Handler::getErrorMessage() const
	{
		return m_errorMessage;
	}

	void Handler::setErrorMessage(const QString& message)
	{
		if (m_errorMessage != message)
		{
			m_errorMessage = message;
			Q_EMIT errorMessageChanged();
		}
	}

	void Handler::start()
	{
		QObject::connect(m_io.get(), &IOBase::started, this, &Handler::onStarted);
		QObject::connect(m_io.get(), &IOBase::startupErrorOccured, this, &Handler::onStartupErrorOccured);
		QObject::connect(m_io.get(), &IOBase::errorOccured, this, &Handler::onErrorOccured);
		m_io->start();
	}

	void Handler::stop()
	{
		if (m_io)
		{
			m_io->stop();
			m_io->disconnect(this);
		}
	}

	void Handler::onStarted()
	{
		HL_INFO(LL, QString("IO started (name=%1)").arg(m_io->uniqueName()).toStdString());
		setState(State::Started);
		setErrorMessage({});
	}

	void Handler::onStartupErrorOccured(const QString& errorMessage)
	{
		HL_ERROR(LL, QString("IO startup error (name=%1) -> %2").arg(m_io->uniqueName()).arg(errorMessage).toStdString());
		setErrorMessage(errorMessage);
		setState(State::Failed);
	}

	void Handler::onErrorOccured(const QString& errorMessage)
	{
		HL_ERROR(LL, QString("IO error (name=%1) -> %2").arg(m_io->uniqueName()).arg(errorMessage).toStdString());
		setErrorMessage(errorMessage);
		setState(State::Failed);
	}
}