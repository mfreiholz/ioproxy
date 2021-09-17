#include "WebSocketClientIO.hpp"
#include "WebSocketHandler.hpp"

class WebSocketClientIO::P
{
public:
};

WebSocketClientIO::WebSocketClientIO()
	: IOBase()
	, d(std::make_unique<P>())
{}

WebSocketClientIO::~WebSocketClientIO()
{}

void WebSocketClientIO::start()
{
}

void WebSocketClientIO::stop()
{
}

void WebSocketClientIO::writeData(const DataPack&)
{
}