#include <humblelogging/humblelogging.h>
#include <ioproxy/Engine.hpp>
#include <ioproxy/Handler.hpp>
using namespace ioproxy;

HUMBLE_LOGGER(L, "ioproxy.handler");

Handler::Handler(Engine* engine, std::unique_ptr<IOBase> io)
	: m_engine(engine)
	, m_io(std::move(io))
{
	// @todo If it should be possible to run IO in a separate thread,
	// we need to move it to the thread right here, bevore we connect
	// any signal/slots.
}

Handler::~Handler()
{
	stop();
}

const std::unique_ptr<IOBase>& Handler::io() const
{
	return m_io;
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
	m_io->stop();
	m_io->disconnect(this);
	// @todo Revert everything done in start().
}

void Handler::onStarted()
{
	HL_INFO(L, QString("IO started (name=%1)").arg(m_io->uniqueName()).toStdString());
}

void Handler::onStartupErrorOccured(const QString& errorMessage)
{
	HL_ERROR(L, QString("IO startup error (name=%1) -> %2").arg(m_io->uniqueName()).arg(errorMessage).toStdString());
}

void Handler::onErrorOccured(const QString& errorMessage)
{
	HL_ERROR(L, QString("IO error (name=%1) -> %2").arg(m_io->uniqueName()).arg(errorMessage).toStdString());
}