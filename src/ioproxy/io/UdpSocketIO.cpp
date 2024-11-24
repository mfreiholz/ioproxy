#include "UdpSocketIO.hpp"
#include "../util/Util.hpp"
#include <QNetworkDatagram>

HUMBLE_LOGGER(LL, "ioproxy.udpsocket");

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

UdpSocketIO::UdpSocketIO(const Options& options)
	: IOBase()
	, m_options(options)
{}

void UdpSocketIO::start()
{
	m_socket = new QUdpSocket(this);

	std::optional<QNetworkInterface> iface;
	QHostAddress bindAddress(QHostAddress::Any);

	// multicast
	if (isMulticastConfig(m_options))
	{
		bindAddress = QHostAddress::AnyIPv4;
		const auto groupPort = m_options.bindPort;

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

		HL_DEBUG(LL, QString("iface=%1").arg(iface ? iface->humanReadableName() : "n/A").toStdString());
		HL_DEBUG(LL, QString("bind=%1").arg(bindAddress.toString()).toStdString());
		HL_DEBUG(LL, QString("group_port=%1").arg(groupPort).toStdString());

		// bind socket
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

		// join all defined multicast addresses
		for (const auto& pair : m_options.remoteAddresses)
		{
			const auto groupAddress = pair.first;

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
				Q_EMIT startupErrorOccured(QString("Can not join multicast on default iface. group=%1; detail=%2")
											   .arg(groupAddress.toString())
											   .arg(m_socket->errorString()));
				return;
			}
			HL_DEBUG(LL, QString("group_address=%1").arg(groupAddress.toString()).toStdString());
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

	QObject::connect(m_socket, &QUdpSocket::connected, this, &UdpSocketIO::onSocketConnected);
	QObject::connect(m_socket, &QUdpSocket::disconnected, this, &UdpSocketIO::onSocketDisconnected);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
	QObject::connect(m_socket, static_cast<void (QUdpSocket::*)(QAbstractSocket::SocketError)>(&QUdpSocket::error), this, &UdpSocketIO::onSocketErrorOccurred);
#else
	QObject::connect(m_socket, &QUdpSocket::errorOccurred, this, &UdpSocketIO::onSocketErrorOccurred);
#endif
	QObject::connect(m_socket, &QUdpSocket::proxyAuthenticationRequired, this, &UdpSocketIO::onSocketProxyAuthenticationRequired);
	QObject::connect(m_socket, &QUdpSocket::readyRead, this, &UdpSocketIO::onReadyRead);
	emit started();
}

void UdpSocketIO::stop()
{
	if (m_socket)
	{
		m_socket->disconnect(this);
		m_socket->close();
		delete m_socket;
		m_socket = nullptr;
	}
}

void UdpSocketIO::writeData(const DataPack& data)
{
	if (m_socket->state() != QAbstractSocket::BoundState)
	{
		HL_ERROR(LL, "Can not write to socket. Not in bound state yet.");
		return;
	}
	// multicast
	if (isMulticastConfig(m_options))
	{
		const auto bytesWritten = m_socket->writeDatagram(data.bytes, multicastGroupAddress(m_options), m_options.bindPort);
		if (bytesWritten < 0)
		{
			HL_ERROR(LL, "Can't write bytes over socket");
		}
		m_statistic.bytesWritten += bytesWritten;
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
			m_statistic.bytesWritten += bytesWritten;
		}
	}
}

bool UdpSocketIO::isMulticastConfig(const Options& options) const
{
	return options.remoteAddresses.size() > 0 && options.remoteAddresses.at(0).first.isMulticast();
}

QHostAddress UdpSocketIO::multicastGroupAddress(const Options& options) const
{
	return options.remoteAddresses.at(0).first;
}

void UdpSocketIO::onSocketConnected()
{
	HL_INFO(LL, "Connected");
}

void UdpSocketIO::onSocketDisconnected()
{
	HL_INFO(LL, "Disconnected");
}

void UdpSocketIO::onSocketErrorOccurred(QAbstractSocket::SocketError socketError)
{
	if (socketError == QAbstractSocket::ConnectionRefusedError)
		return; //< Do not forward as error, if there is no remote listener yet.
	Q_EMIT errorOccured(m_socket->errorString());
}

void UdpSocketIO::onSocketProxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)
{
	HL_INFO(LL, "Socket requires proxy authentication (not supported yet)");
}

void UdpSocketIO::onReadyRead()
{
	while (m_socket->hasPendingDatagrams())
	{
		const QNetworkDatagram dgram = m_socket->receiveDatagram();
		if (m_options.echo)
		{
			auto written = m_socket->writeDatagram(dgram.data(), dgram.senderAddress(), dgram.senderPort());
			m_statistic.bytesWritten += written;
		}
		DataPack data(dgram.data());
		data.fixedSize = data.bytes.size();
		data.parameters.insert("sender_address", dgram.senderAddress().toString());
		data.parameters.insert("sender_port", dgram.senderPort());
		m_statistic.bytesRead += data.bytes.size();
		emit newData(data);
	}
}

///////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////

UdpSocketIOFactory::UdpSocketIOFactory()
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

	def.key = "remote_address";
	def.description = "IPv4/IPv6 address of remote machine.";
	def.valueDataType = IOParaDef::STRING;
	def.required = false;
	def.multi = true;
	addParaDef(def);

	def.key = "remote_port";
	def.description = "Port of remote machine.";
	def.valueDataType = IOParaDef::INT16;
	def.required = false;
	def.multi = true;
	addParaDef(def);

	def.key = "echo";
	def.description = "Sends received data back to sender.";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.multi = false;
	def.defaultValue = false;
	addParaDef(def);

	def.key = "multicast_ttl";
	def.description = "TTL for multicast datagrams.";
	def.valueDataType = IOParaDef::INT8;
	def.required = false;
	def.multi = false;
	def.defaultValue = 0;
	addParaDef(def);

	def.key = "multicast_loopback";
	def.description = "If \"true\" the loopback for multicast packets is enabled.";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.multi = false;
	def.defaultValue = false;
	addParaDef(def);
}

QString UdpSocketIOFactory::getID() const
{
	return ID;
}

tl::expected<IOBase*, QString> UdpSocketIOFactory::create(const IOParameters& params)
{
	auto eParaCheck = checkRequiredParameters(params);
	if (!eParaCheck)
		return tl::make_unexpected(eParaCheck.error());

	UdpSocketIO::Options opts;
	opts.bindAddress = QHostAddress(getParaValue(params, m_paraDefsByKey["bind_address"]).value().toString());
	opts.bindPort = getParaValue(params, m_paraDefsByKey["bind_port"]).value().toUInt();

	// We allow multiple remote hosts.
	int index = 0;
	while (true)
	{
		auto eRemoteAddress = getParaValue(params, m_paraDefsByKey["remote_address"], index);
		if (!eRemoteAddress)
			break;

		auto eRemotePort = getParaValue(params, m_paraDefsByKey["remote_port"], index);
		if (!eRemotePort)
			return tl::make_unexpected(QString("Missing remote_port parameter for defined remote_address (addr=%1; index=%2).").arg(eRemoteAddress.value().toString()).arg(index));

		QHostAddress addr(eRemoteAddress.value().toString());
		int port = eRemotePort.value().toUInt();
		opts.remoteAddresses.push_back(std::make_pair(addr, port));
		index++;
	}

	opts.echo = getParaValue(params, m_paraDefsByKey["echo"]).value().toBool();
	opts.multicastTTL = getParaValue(params, m_paraDefsByKey["multicast_ttl"]).value().toUInt();
	opts.multicastLoopback = getParaValue(params, m_paraDefsByKey["multicast_loopback"]).value().toBool();
	return {new UdpSocketIO(opts)};
}