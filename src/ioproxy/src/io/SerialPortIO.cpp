#include <humblelogging/humblelogging.h>
#include <ioproxy/io/SerialPortIO.hpp>

HUMBLE_LOGGER(L, "ioproxy.serialport");

using namespace ioproxy;

const QString SerialPortIO::TYPE = "serialport";