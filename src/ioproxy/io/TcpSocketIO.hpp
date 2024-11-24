#pragma once
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QHostAddress>
#include <QTcpSocket>
#include <cinttypes>
#include <memory>

class TcpSocketIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QHostAddress remoteAddress;
		uint16_t remotePort;
		bool reconnect = false;

		// If `true` it sets the TCP_NODELAY option on socket.
		// Disables the Nagle's algorithm.
		bool tcpNoDelayOption = false;

		// If `true` it sets the SO_KEEPALIVE option on socket.
		bool tcpKeepAliveOption = false;
	};

	explicit TcpSocketIO(const Options& opts);
	TcpSocketIO(const TcpSocketIO&) = delete;
	TcpSocketIO& operator=(const TcpSocketIO&) = delete;
	~TcpSocketIO() override;
	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

private Q_SLOTS:
	void connectToRemote();
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketErrorOccured(QAbstractSocket::SocketError);
	void onSocketReadyRead();
	void reconnect();

private:
	Options m_options;
	std::unique_ptr<QTcpSocket> m_socket;
};

namespace ioproxy
{
	class TcpSocketFactory : public BuiltInIOFactoryBase
	{
	public:
		inline static const QString ID = QString("tcpsocket");

		TcpSocketFactory();
		QString getID() const override;
		tl::expected<IOBase*, QString> create(const IOParameters& parameters);
	};
}