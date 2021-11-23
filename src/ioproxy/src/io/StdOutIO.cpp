#include <humblelogging/humblelogging.h>
#include <ioproxy/io/StdOutIO.hpp>

HUMBLE_LOGGER(L, "ioproxy.stdout");

using namespace ioproxy;

const QString StdOutIO::TYPE = "stdout";

StdOutIO::StdOutIO()
	: IOBase()
{}

StdOutIO::~StdOutIO()
{
	stop();
}

void StdOutIO::setOptions(const Options& options)
{
	m_options = options;
}

void StdOutIO::start()
{
	emit started();
}

void StdOutIO::stop()
{
}

void StdOutIO::writeData(const DataPack& data)
{
	QString str(data.bytes);
	auto s = str.toStdString();
	std::cout << s;

	m_statistic.bytesWritten += str.size();
}