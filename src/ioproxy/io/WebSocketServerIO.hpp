#pragma once
#include "../Global.hpp"
#include "IOBase.hpp"
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
	};

	WebSocketServerIO(const Options& options);
	~WebSocketServerIO() = default;
	WebSocketServerIO(const WebSocketServerIO&) = delete;
	WebSocketServerIO& operator=(const WebSocketServerIO&) = delete;

	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

protected Q_SLOTS:

	/* QWebSocketServer */

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

	/* QWebSocket */

	void onWebSocketDisconnected();
	void onWebSocketBinaryMessageReceived(const QByteArray& message);
	void onWebSocketTextMessageReceived(const QString& message);

private:
	Options m_options;
	QWebSocketServer* m_server = nullptr;
	QList<QWebSocket*> m_sockets;
};