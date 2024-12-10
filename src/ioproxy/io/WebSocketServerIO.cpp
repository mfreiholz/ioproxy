#include "WebSocketServerIO.hpp"

WebSocketServerIO::WebSocketServerIO(const Options& options)
	: IOBase()
	, m_options(options)
{}

void WebSocketServerIO::start()
{
	m_server = new QWebSocketServer("ServerName", QWebSocketServer::SslMode::NonSecureMode, this);
	if (!m_server->listen(m_options.bindAddress, m_options.bindPort))
	{
		Q_EMIT errorOccured(m_server->errorString());
		return;
	}
}

void WebSocketServerIO::stop()
{}

void WebSocketServerIO::writeData(const DataPack& data)
{
	// TODO Check with: QWebSocket::maxOutgoingFrameSize()?
	for (QWebSocket* ws : m_sockets)
	{
		if (m_options.binary)
		{
			ws->sendBinaryMessage(data.bytes);
		}
		else
		{
			// TODO Support multiple encodings.
			ws->sendTextMessage(QString::fromUtf8(data.bytes));
		}
	}
}

void WebSocketServerIO::onNewConnection()
{
	while (m_server->hasPendingConnections())
	{
		QWebSocket* ws = m_server->nextPendingConnection();
		QObject::connect(ws, &QWebSocket::disconnected, this, &WebSocketServerIO::onWebSocketDisconnected);
		m_sockets.append(ws);
	}
}

void WebSocketServerIO::onWebSocketDisconnected()
{
	QWebSocket* ws = qobject_cast<QWebSocket*>(sender());
	m_sockets.removeOne(ws);
	ws->deleteLater();
}

void WebSocketServerIO::onWebSocketBinaryMessageReceived(const QByteArray& message)
{
}

void WebSocketServerIO::onWebSocketTextMessageReceived(const QString& message)
{
}