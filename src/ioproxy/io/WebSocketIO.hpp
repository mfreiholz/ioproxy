#pragma once
#include "../Global.hpp"
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QWebSocket>

class WebSocketIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QUrl remoteUrl;
		bool reconnect = false;
		bool binary = false;
	};

	WebSocketIO(const Options& options);
	~WebSocketIO() = default;
	WebSocketIO(const WebSocketIO&) = delete;
	WebSocketIO& operator=(const WebSocketIO&) = delete;

	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

private Q_SLOTS:
	void connectToRemote();
	void reconnect();

	void onAboutToClose();
	void onAlertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description);
	void onAlertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString& description);
	void onAuthenticationRequired(QAuthenticator* authenticator);
	void onBinaryMessageReceived(const QByteArray& message);
	void onConnected();
	void onDisconnected();
	void onSocketErrorOccured(QAbstractSocket::SocketError);
	void onHandshakeInterruptedOnError(const QSslError& error);
	void onPeerVerifyError(const QSslError& error);
	void onPong(quint64 elapsedTime, const QByteArray& payload);
	void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator* authenticator);
	void onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);
	void onSslErrors(const QList<QSslError>& errors);
	void onTextMessageReceived(const QString& message);

private:
	Options m_options;
	QWebSocket* m_socket = nullptr;
};


class WebSocketIOFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("websocket");

	WebSocketIOFactory();
	QString getID() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};