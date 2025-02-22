#include "Handler.hpp"
#include <humblelogging/humblelogging.h>

HUMBLE_LOGGER(LL, "ioproxy");

namespace ioproxy
{
	Handler::Handler(std::shared_ptr<IOBase> io, QObject* parent)
		: QObject(parent)
		, m_io(io)
	{
		QObject::connect(m_io.get(), &IOBase::newData, this, &Handler::onNewData);
	}

	Handler::~Handler()
	{
		stop();
	}

	QString Handler::uniqueName() const
	{
		return m_io->uniqueName();
	}

	Handler::State Handler::state() const
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

	QString Handler::errorMessage() const
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

	qint64 Handler::bytesWritten() const
	{
		return m_bytesWritten;
	}

	qint64 Handler::bytesRead() const
	{
		return m_bytesRead;
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

	void Handler::writeData(const DataPack& data)
	{
		if (m_state != Started)
			return;

		m_io->writeData(data);

		m_bytesWritten += data.bytes.count();
	}

	void Handler::onStarted()
	{
		HL_INFO(LL, QString("IO started (name=%1)").arg(m_io->uniqueName()).toStdString());
		setErrorMessage({});
		setState(State::Started);
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

	void Handler::onNewData(const DataPack& data)
	{
		m_bytesRead += data.bytes.count();
		Q_EMIT newData(data);
	}
}