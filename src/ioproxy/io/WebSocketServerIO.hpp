#pragma once
#include "../Global.hpp"
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QWebSocket>
#include <QWebSocketServer>

class WebSocketServerIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QHostAddress bindAddress;
		quint16 bindPort;
		bool binary = false;
		QString serverName;
	};

	WebSocketServerIO(const Options& options);
	~WebSocketServerIO() = default;
	WebSocketServerIO(const WebSocketServerIO&) = delete;
	WebSocketServerIO& operator=(const WebSocketServerIO&) = delete;

	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

protected Q_SLOTS:
	void onAcceptError(QAbstractSocket::SocketError socketError);
	void onAlertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description);
	void onAlertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description);
	void onClosed();
	void onHandshakeInterruptedOnError(const QSslError& error);
	void onNewConnection();
	void onOriginAuthenticationRequired(QWebSocketCorsAuthenticator* authenticator);
	void onPeerVerifyError(const QSslError& error);
	void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator* authenticator);
	void onServerError(QWebSocketProtocol::CloseCode closeCode);
	void onSslErrors(const QList<QSslError>& errors);

	void onWebSocketDisconnected();
	void onWebSocketBinaryMessageReceived(const QByteArray& message);
	void onWebSocketTextMessageReceived(const QString& message);

private:
	Options m_options;
	QWebSocketServer* m_server = nullptr;
	QList<QWebSocket*> m_sockets;
};


/*! ... */
class WebSocketServerIOFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("websocketserver");

	WebSocketServerIOFactory();
	QString getID() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};