#include "WebSocketServerIO.hpp"

HUMBLE_LOGGER(LL, "ioproxy.websocketserver");

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

WebSocketServerIO::WebSocketServerIO(const Options& options)
	: IOBase()
	, m_options(options)
{}

void WebSocketServerIO::start()
{
	m_server = new QWebSocketServer(m_options.serverName, QWebSocketServer::SslMode::NonSecureMode, this);
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

void WebSocketServerIO::onAcceptError(QAbstractSocket::SocketError socketError)
{
	Q_EMIT errorOccured(QString("Accept error (err=%1)").arg(socketError));
}

void WebSocketServerIO::onAlertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description)
{
	Q_EMIT errorOccured(QString("Alert received (level=%1; type=%2; description=%3)").arg(static_cast<int>(level)).arg(static_cast<int>(type)).arg(description));
}

void WebSocketServerIO::onAlertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description)
{
	Q_EMIT errorOccured(QString("Alert sent (level=%1; type=%2; description=%3)").arg(static_cast<int>(level)).arg(static_cast<int>(type)).arg(description));
}

void WebSocketServerIO::onClosed()
{
}

void WebSocketServerIO::onHandshakeInterruptedOnError(const QSslError& error)
{
	Q_EMIT errorOccured(QString("Handshake interrupted error: %1").arg(error.errorString()));
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

void WebSocketServerIO::onOriginAuthenticationRequired(QWebSocketCorsAuthenticator* authenticator)
{
}

void WebSocketServerIO::onPeerVerifyError(const QSslError& error)
{
}

void WebSocketServerIO::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator* authenticator)
{
}

void WebSocketServerIO::onServerError(QWebSocketProtocol::CloseCode closeCode)
{
}

void WebSocketServerIO::onSslErrors(const QList<QSslError>& errors)
{
}

void WebSocketServerIO::onWebSocketDisconnected()
{
	QWebSocket* ws = qobject_cast<QWebSocket*>(sender());
	m_sockets.removeOne(ws);
	ws->deleteLater();
}

void WebSocketServerIO::onWebSocketBinaryMessageReceived(const QByteArray& message)
{
	Q_EMIT newData(message);
}

void WebSocketServerIO::onWebSocketTextMessageReceived(const QString& message)
{
	Q_EMIT newData(message.toUtf8());
}

///////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////

WebSocketServerIOFactory::WebSocketServerIOFactory()
	: BuiltInIOFactoryBase()
{
	IOParaDef def;

	def.key = "bind_address";
	def.description = "IPv4/IPv6 address of local network interface to bind socket.";
	def.valueDataType = IOParaDef::STRING;
	def.required = false;
	def.multi = false;
	def.defaultValue = QString("0.0.0.0");
	addParaDef(def);

	def.key = "bind_port";
	def.description = "Port number on which to listen for incoming packets. In multicast mode this is the group-port.";
	def.valueDataType = IOParaDef::INT16;
	def.required = false;
	def.multi = false;
	def.defaultValue = 0;
	addParaDef(def);

	def.key = "binary";
	def.description = "Indicates whether the transfered data should be handled as binary message.";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.multi = false;
	def.defaultValue = false;
	addParaDef(def);

	def.key = "server_name";
	def.description = "Name of the server.";
	def.valueDataType = IOParaDef::STRING;
	def.required = false;
	def.multi = false;
	def.defaultValue = QString("IOProxy");
	addParaDef(def);
}

QString WebSocketServerIOFactory::getID() const
{
	return ID;
}

tl::expected<IOBase*, QString> WebSocketServerIOFactory::create(const IOParameters& params)
{
	auto eParaCheck = checkRequiredParameters(params);
	if (!eParaCheck)
		return tl::make_unexpected(eParaCheck.error());

	WebSocketServerIO::Options opts;
	opts.bindAddress = QHostAddress(getParaValue(params, m_paraDefsByKey["bind_address"]).value().toString());
	opts.bindPort = getParaValue(params, m_paraDefsByKey["bind_port"]).value().toUInt();
	opts.binary = getParaValue(params, m_paraDefsByKey["binary"]).value().toBool();
	opts.serverName = getParaValue(params, m_paraDefsByKey["server_name"]).value().toString();
	return {new WebSocketServerIO(opts)};
}