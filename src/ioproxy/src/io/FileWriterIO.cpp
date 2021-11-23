#include <humblelogging/humblelogging.h>
#include <ioproxy/io/FileWriterIO.hpp>

HUMBLE_LOGGER(L, "ioproxy.filewriter");

using namespace ioproxy;

const QString FileWriterIO::TYPE = "filewriter";