#include <humblelogging/humblelogging.h>
#include <ioproxy/io/CompressionIO.hpp>

HUMBLE_LOGGER(L, "ioproxy.compression");

namespace ioproxy
{
	const QString CompressionIO::TYPE = "compression";
}