#include <ioproxy/IOHandler.hpp>

IOHandler::IOHandler(std::shared_ptr<IOBase> io_)
	: QObject()
	, m_io(io_)
{
	QObject::connect(m_io.get(), &IOBase::newData, this, &IOHandler::onNewData);
}

IOHandler::~IOHandler()
{
}

std::shared_ptr<IOBase> IOHandler::io() const
{
	return m_io;
}