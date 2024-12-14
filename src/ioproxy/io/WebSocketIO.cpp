#include "WebSocketIO.hpp"
#include <QTimer>

HUMBLE_LOGGER(LL, "ioproxy.websocket");

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

WebSocketIO::WebSocketIO(const Options& options)
	: IOBase()
	, m_options(options)
{}

void WebSocketIO::start()
{
	connectToRemote();
	Q_EMIT started();
}

void WebSocketIO::stop()
{
	if (m_socket)
	{
		m_socket->disconnect(this);
		m_socket->close();
		m_socket->deleteLater();
		m_socket = nullptr;
	}
}

void WebSocketIO::writeData(const DataPack& data)
{
	if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState || !m_socket->isValid())
	{
		return;
	}

	qint64 written = 0;
	if (m_options.binary)
	{
		written = m_socket->sendBinaryMessage(data.bytes);
		if (written != data.bytes.size())
		{
			Q_EMIT errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
									.arg(written)
									.arg(data.bytes.size())
									.arg(m_socket->peerAddress().toString())
									.arg(m_socket->peerPort()));
			return;
		}
	}
	else
	{
		written = m_socket->sendTextMessage(QString::fromUtf8(data.bytes));
		if (written != data.bytes.size())
		{
			Q_EMIT errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
									.arg(written)
									.arg(data.bytes.size())
									.arg(m_socket->peerAddress().toString())
									.arg(m_socket->peerPort()));
			return;
		}
	}

	// TODO See QAbstractSocket::waitForBytesWritten(-1)..
	// We might need it in case of memory problems.
	m_statistic.bytesWritten += written;
}

void WebSocketIO::connectToRemote()
{
	stop();
	m_socket = new QWebSocket(QString(), QWebSocketProtocol::Version::VersionLatest, this);
	QObject::connect(m_socket, &QWebSocket::aboutToClose, this, &WebSocketIO::onAboutToClose);
	QObject::connect(m_socket, &QWebSocket::alertReceived, this, &WebSocketIO::onAlertReceived);
	QObject::connect(m_socket, &QWebSocket::alertSent, this, &WebSocketIO::onAlertSent);
	QObject::connect(m_socket, &QWebSocket::authenticationRequired, this, &WebSocketIO::onAuthenticationRequired);
	QObject::connect(m_socket, &QWebSocket::binaryMessageReceived, this, &WebSocketIO::onBinaryMessageReceived);
	QObject::connect(m_socket, &QWebSocket::connected, this, &WebSocketIO::onConnected);
	QObject::connect(m_socket, &QWebSocket::disconnected, this, &WebSocketIO::onDisconnected);
	QObject::connect(m_socket, &QWebSocket::errorOccurred, this, &WebSocketIO::onSocketErrorOccured);
	QObject::connect(m_socket, &QWebSocket::handshakeInterruptedOnError, this, &WebSocketIO::onHandshakeInterruptedOnError);
	QObject::connect(m_socket, &QWebSocket::peerVerifyError, this, &WebSocketIO::onPeerVerifyError);
	QObject::connect(m_socket, &QWebSocket::pong, this, &WebSocketIO::onPong);
	QObject::connect(m_socket, &QWebSocket::preSharedKeyAuthenticationRequired, this, &WebSocketIO::onPreSharedKeyAuthenticationRequired);
	QObject::connect(m_socket, &QWebSocket::proxyAuthenticationRequired, this, &WebSocketIO::onProxyAuthenticationRequired);
	QObject::connect(m_socket, &QWebSocket::sslErrors, this, &WebSocketIO::onSslErrors);
	QObject::connect(m_socket, &QWebSocket::textMessageReceived, this, &WebSocketIO::onTextMessageReceived);
	m_socket->open(m_options.remoteUrl);
}

void WebSocketIO::reconnect()
{
	QTimer::singleShot(500, this, &WebSocketIO::connectToRemote);
}

void WebSocketIO::onAboutToClose()
{
}

void WebSocketIO::onAlertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description)
{
}

void WebSocketIO::onAlertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description)
{
}

void WebSocketIO::onAuthenticationRequired(QAuthenticator* authenticator)
{
}

void WebSocketIO::onBinaryMessageReceived(const QByteArray& message)
{
	Q_EMIT newData(message);
}

void WebSocketIO::onConnected()
{
}

void WebSocketIO::onDisconnected()
{
}

void WebSocketIO::onSocketErrorOccured(QAbstractSocket::SocketError)
{
	Q_EMIT errorOccured(QString("Error for connection to %1 (%2)")
							.arg(m_options.remoteUrl.toString())
							.arg(m_socket->errorString()));
	if (m_options.reconnect)
	{
		reconnect();
	}
}

void WebSocketIO::onHandshakeInterruptedOnError(const QSslError& error)
{
}

void WebSocketIO::onPeerVerifyError(const QSslError& error)
{
}

void WebSocketIO::onPong(quint64 elapsedTime, const QByteArray& payload)
{
}

void WebSocketIO::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator* authenticator)
{
}

void WebSocketIO::onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
}

void WebSocketIO::onSslErrors(const QList<QSslError>& errors)
{
}

void WebSocketIO::onTextMessageReceived(const QString& message)
{
	Q_EMIT newData(message.toUtf8());
}

///////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////

WebSocketIOFactory::WebSocketIOFactory()
	: BuiltInIOFactoryBase()
{
	IOParaDef def;

	def.key = "remote_url";
	def.description = "URL of the remote host.";
	def.valueDataType = IOParaDef::STRING;
	def.required = true;
	def.multi = false;
	addParaDef(def);

	def.key = "reconnect";
	def.description = "If 'true' the socket automatically tries to reconnect.";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.multi = false;
	def.defaultValue = {true};
	addParaDef(def);

	def.key = "binary";
	def.description = "Indicates whether the transfered data should be handled as binary message.";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.multi = false;
	def.defaultValue = false;
	addParaDef(def);
}

QString WebSocketIOFactory::getID() const
{
	return ID;
}

tl::expected<IOBase*, QString> WebSocketIOFactory::create(const IOParameters& params)
{
	auto eParaCheck = checkRequiredParameters(params);
	if (!eParaCheck)
		return tl::make_unexpected(eParaCheck.error());

	WebSocketIO::Options opts;
	opts.remoteUrl = QUrl(getParaValue(params, m_paraDefsByKey["remote_url"]).value().toString());
	opts.reconnect = getParaValue(params, m_paraDefsByKey["reconnect"]).value().toBool();
	opts.binary = getParaValue(params, m_paraDefsByKey["binary"]).value().toBool();
	return {new WebSocketIO(opts)};
}