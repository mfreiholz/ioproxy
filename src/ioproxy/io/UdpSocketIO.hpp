#pragma once
#include "../Global.hpp"
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QHostAddress>
#include <QUdpSocket>
#include <optional>
#include <utility>
#include <vector>

/*
	Capabilities:
	Can receive data on "bindAdress:bindPort".

	@todo Allow to define max/fixed datagram size.
*/
class UdpSocketIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		// The local address to bind on.
		// Multicast: Used to find the responsible network interface.
		std::optional<QHostAddress> bindAddress;

		// Port to bind. If `bindPort == 0`, the OS will select a random free number.
		uint16_t bindPort = 0;

		// Allow multiple remote
		std::vector<std::pair<QHostAddress, uint16_t>> remoteAddresses;

		// Send back received packet to sender
		bool echo = false;

		std::optional<uint16_t> multicastTTL;
		bool multicastLoopback = false;
	};

	UdpSocketIO(const Options& options);
	~UdpSocketIO() override = default;
	UdpSocketIO(const UdpSocketIO&) = delete;
	UdpSocketIO& operator=(const UdpSocketIO&) = delete;

	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

protected:
	bool isMulticastConfig(const Options& options) const;
	QHostAddress multicastGroupAddress(const Options& options) const;

private Q_SLOTS:
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketErrorOccurred(QAbstractSocket::SocketError socketError);
	void onSocketProxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*);
	void onReadyRead();

private:
	Options m_options;
	QUdpSocket* m_socket = nullptr;
};


/*! ... */
class UdpSocketIOFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("udpsocket");

	UdpSocketIOFactory();
	QString getID() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};