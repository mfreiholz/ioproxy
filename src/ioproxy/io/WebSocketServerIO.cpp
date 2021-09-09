#include "WebSocketServerIO.hpp"
#include "WebSocketHandler.hpp"
#include <QWebSocketServer>

class WebSocketServerIO::P
{
public:
};

WebSocketServerIO::WebSocketServerIO()
	: IOBase()
	, d(std::make_unique<P>())
{}

WebSocketServerIO::~WebSocketServerIO()
{}

void WebSocketServerIO::start()
{
}

void WebSocketServerIO::stop()
{
}

void WebSocketServerIO::writeData(const QByteArray&)
{
}