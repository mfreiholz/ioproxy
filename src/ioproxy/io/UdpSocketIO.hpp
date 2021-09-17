#pragma once
#include "../Global.hpp"
#include "../IOBase.hpp"
#include "../Util.hpp"
#include <QAuthenticator>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QNetworkProxy>
#include <QUdpSocket>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>

/*
	Capabilities:
	Can receive data on "bindAdress:bindPort".

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
		uint16_t bindPort;

		// Allow multiple remote
		std::vector<std::pair<QHostAddress, uint16_t>> remoteAddresses;

		std::optional<uint16_t> multicastTTL;
		bool multicastLoopback = false;
	};

public:
	UdpSocketIO()
		: IOBase()
	{}

	~UdpSocketIO() override
	{
		this->stop();
	}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start() override
	{
		m_socket = std::make_unique<QUdpSocket>();

		std::optional<QNetworkInterface> iface;
		QHostAddress bindAddress(QHostAddress::Any);

		// multicast
		if (isMulticastConfig(m_options))
		{
			bindAddress = QHostAddress::AnyIPv4;

			// check for IPv4 or IPv6
			if (m_options.bindAddress.has_value() && !m_options.bindAddress.value().isNull())
			{
				const auto addr = m_options.bindAddress.value();
				iface = Util::findNetworkInterfaceByIP(addr);
				if (addr.protocol() == QAbstractSocket::IPv4Protocol)
				{
					bindAddress = QHostAddress::AnyIPv4;
				}
				else if (addr.protocol() == QAbstractSocket::IPv6Protocol)
				{
					bindAddress = QHostAddress::AnyIPv6;
				}
			}

			HL_INFO(LL, QString("iface=%1").arg(iface ? iface->humanReadableName() : "n/A").toStdString());
			HL_INFO(LL, QString("bind=%1").arg(bindAddress.toString()).toStdString());

			// bind socket
			const auto groupPort = multicastGroupPort(m_options);
			if (!m_socket->bind(bindAddress, groupPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
			{
				emit startupErrorOccured(QString("Can't bind local socket: %1:%2 (Qt-error=%3)").arg(bindAddress.toString()).arg(QString::number(groupPort)).arg(m_socket->errorString()));
				return;
			}

			// loopback on/off?
			m_socket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, m_options.multicastLoopback ? 1 : 0);

			// tll allows to reach other networks via gateways
			if (m_options.multicastTTL)
				m_socket->setSocketOption(QAbstractSocket::MulticastTtlOption, m_options.multicastTTL.value());

			// set outgoing multicast iface
			if (iface)
				m_socket->setMulticastInterface(iface.value());

			// join multicast group
			const auto groupAddress = multicastGroupAddress(m_options);

			// try to join on specific iface, before falling back on default iface.
			bool joinOnIfaceFailed = false;
			if (iface)
			{
				if (!m_socket->joinMulticastGroup(groupAddress, iface.value()))
				{
					HL_WARN(LL, QString("Can not join multicast on iface. group=%1; iface=%2; detail=%3")
									.arg(groupAddress.toString())
									.arg(iface->humanReadableName())
									.arg(m_socket->errorString())
									.toStdString());
					joinOnIfaceFailed = true;
				}
			}

			if ((!iface || joinOnIfaceFailed) && !m_socket->joinMulticastGroup(groupAddress))
			{
				emit startupErrorOccured(QString("Can not join multicast on default iface. group=%1; detail=%2")
											 .arg(groupAddress.toString())
											 .arg(m_socket->errorString()));
				return;
			}
		}
		// unicast
		else
		{
			// check for optional binding address
			if (m_options.bindAddress.has_value())
			{
				bindAddress = m_options.bindAddress.value();
			}

			// bind socket
			const auto bindPort = std::clamp(m_options.bindPort, static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
			if (!m_socket->bind(bindAddress, bindPort))
			{
				emit startupErrorOccured(QString("Can't bind local socket: %1:%2 (Qt-error=%3)").arg(bindAddress.toString()).arg(QString::number(bindPort)).arg(m_socket->errorString()));
				return;
			}
		}

		QObject::connect(m_socket.get(), &QUdpSocket::connected, this, &UdpSocketIO::onSocketConnected);
		QObject::connect(m_socket.get(), &QUdpSocket::disconnected, this, &UdpSocketIO::onSocketDisconnected);
		QObject::connect(m_socket.get(), &QUdpSocket::errorOccurred, this, &UdpSocketIO::onSocketErrorOccurred);
		QObject::connect(m_socket.get(), &QUdpSocket::proxyAuthenticationRequired, this, &UdpSocketIO::onSocketProxyAuthenticationRequired);
		QObject::connect(m_socket.get(), &QUdpSocket::readyRead, this, &UdpSocketIO::onReadyRead);
		emit started();
	}

	void stop() override
	{
		if (m_socket)
		{
			m_socket->disconnect(this);
			m_socket->close();
			m_socket.reset();
		}
	}

	void writeData(const DataPack& data) override
	{
		if (m_socket->state() != QAbstractSocket::BoundState)
		{
			HL_ERROR(LL, "Can not write to socket. Not in bound state yet.");
			return;
		}
		// multicast
		if (isMulticastConfig(m_options))
		{
			const auto bytesWritten = m_socket->writeDatagram(data.bytes, multicastGroupAddress(m_options), multicastGroupPort(m_options));
			if (bytesWritten < 0)
			{
				HL_ERROR(LL, "Can't write bytes over socket");
			}
		}
		// unicast
		else
		{
			for (const auto& addr : m_options.remoteAddresses)
			{
				const auto bytesWritten = m_socket->writeDatagram(data.bytes, addr.first, addr.second);
				if (bytesWritten < 0)
				{
					HL_ERROR(LL, "Can't write bytes over socket");
				}
			}
		}
	}

protected:
	bool isMulticastConfig(const Options& options) const
	{
		return options.remoteAddresses.size() > 0 && options.remoteAddresses.at(0).first.isMulticast();
	}

	QHostAddress multicastGroupAddress(const Options& options) const
	{
		return options.remoteAddresses.at(0).first;
	}

	uint16_t multicastGroupPort(const Options& options) const
	{
		return std::clamp(options.remoteAddresses.at(0).second, static_cast<uint16_t>(1), std::numeric_limits<uint16_t>::max());
	}

private slots:
	void onSocketConnected()
	{
		HL_INFO(LL, "Connected");
	}

	void onSocketDisconnected()
	{
		HL_INFO(LL, "Disconnected");
	}

	void onSocketErrorOccurred(QAbstractSocket::SocketError)
	{
		emit errorOccured(m_socket->errorString());
	}

	void onSocketProxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)
	{
		HL_INFO(LL, "Socket requires proxy authentication (not supported yet)");
	}

	void onReadyRead()
	{
		while (m_socket->hasPendingDatagrams())
		{
			const QNetworkDatagram dgram = m_socket->receiveDatagram();

			DataPack data(dgram.data());
			data.fixedSize = data.bytes.size();
			data.parameters.insert("sender_address", dgram.senderAddress().toString());
			data.parameters.insert("sender_port", dgram.senderPort());
			emit newData(dgram.data());
		}
	}

private:
	Options m_options;
	std::unique_ptr<QUdpSocket> m_socket;
};