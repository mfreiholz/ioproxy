#include "ioproxy/io/TcpSocketIO.hpp"
#include <QTimer>
#include <humblelogging/humblelogging.h>

HUMBLE_LOGGER(LL, "ioproxy.tcpsocket");

///////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////

TcpSocketIO::TcpSocketIO(const Options& opts)
	: IOBase()
	, m_options(opts)
{}

TcpSocketIO::~TcpSocketIO()
{
	stop();
}

void TcpSocketIO::start()
{
	connectToRemote();
	Q_EMIT started();
}

void TcpSocketIO::stop()
{
	if (m_socket)
	{
		m_socket->disconnect(this);
		m_socket->close();
		m_socket.reset(nullptr);
	}
}

void TcpSocketIO::writeData(const DataPack& data)
{
	if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
	{
		return;
	}

	auto written = m_socket->write(data.bytes);
	if (written != data.bytes.size())
	{
		Q_EMIT errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
								.arg(written)
								.arg(data.bytes.size())
								.arg(m_socket->peerAddress().toString())
								.arg(m_socket->peerPort()));
		return;
	}

	// Fixed: Memory Leak.
	// If we don't call "waitForBytesWritten()", a lot of data will collect in
	// output buffer and fill up the memory. Thats why we wait until the bytes have been written.
	// We use "-1" to never timeout.
	m_socket->waitForBytesWritten(-1);


	m_statistic.bytesWritten += written;
}

void TcpSocketIO::connectToRemote()
{
	stop();
	m_socket = std::make_unique<QTcpSocket>();
	QObject::connect(m_socket.get(), &QTcpSocket::connected, this, &TcpSocketIO::onSocketConnected);
	QObject::connect(m_socket.get(), &QTcpSocket::disconnected, this, &TcpSocketIO::onSocketDisconnected);
	QObject::connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &TcpSocketIO::onSocketErrorOccured);
	QObject::connect(m_socket.get(), &QTcpSocket::readyRead, this, &TcpSocketIO::onSocketReadyRead);
	m_socket->connectToHost(m_options.remoteAddress, m_options.remotePort);
}

void TcpSocketIO::onSocketConnected()
{
	HL_INFO(LL, QString("Connected to remote %1:%2").arg(m_options.remoteAddress.toString()).arg(m_options.remotePort).toStdString());
	if (m_options.tcpNoDelayOption)
		m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	if (m_options.tcpKeepAliveOption)
		m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
}

void TcpSocketIO::onSocketDisconnected()
{
}

void TcpSocketIO::onSocketErrorOccured(QAbstractSocket::SocketError)
{
	Q_EMIT errorOccured(QString("Error for connection to %1:%2 (%3)")
							.arg(m_options.remoteAddress.toString())
							.arg(m_options.remotePort)
							.arg(m_socket->errorString()));
	if (m_options.reconnect)
	{
		reconnect();
	}
}

void TcpSocketIO::onSocketReadyRead()
{
	while (m_socket->bytesAvailable() > 0)
	{
		DataPack data(m_socket->read(m_socket->bytesAvailable()));
		m_statistic.bytesRead += data.bytes.size();
		Q_EMIT newData(data);
	}
}

void TcpSocketIO::reconnect()
{
	HL_INFO(LL, "Initiate auto reconnect");
	QTimer::singleShot(500, this, &TcpSocketIO::connectToRemote);
}

namespace ioproxy
{
	///////////////////////////////////////////////////////////////////
	// Factory
	///////////////////////////////////////////////////////////////////

	TcpSocketFactory::TcpSocketFactory()
		: BuiltInIOFactoryBase()
	{
		IOParaDef def;

		def.key = "remote_address";
		def.description = "Address of the remote host.";
		def.valueDataType = IOParaDef::STRING;
		def.required = true;
		def.multi = false;
		addParaDef(def);

		def.key = "remote_port";
		def.description = "Port of the remote host.";
		def.valueDataType = IOParaDef::INT16;
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

		def.key = "low_delay";
		def.description = "";
		def.valueDataType = IOParaDef::BOOLEAN;
		def.required = false;
		def.multi = false;
		def.defaultValue = {false};
		addParaDef(def);

		def.key = "keep_alive";
		def.description = "";
		def.valueDataType = IOParaDef::BOOLEAN;
		def.required = false;
		def.multi = false;
		def.defaultValue = {false};
		addParaDef(def);
	}

	QString TcpSocketFactory::getID() const
	{
		return TcpSocketFactory::ID;
	}

	tl::expected<IOBase*, QString> TcpSocketFactory::create(const IOParameters& params)
	{
		auto eParaCheck = checkRequiredParameters(params);
		if (!eParaCheck)
			return tl::make_unexpected(eParaCheck.error());

		TcpSocketIO::Options opts;
		opts.remoteAddress = QHostAddress(getParaValue(params, m_paraDefsByKey["remote_address"]).value().toString());
		opts.remotePort = getParaValue(params, m_paraDefsByKey["remote_port"]).value().toUInt();
		opts.reconnect = getParaValue(params, m_paraDefsByKey["reconnect"]).value().toBool();
		opts.tcpNoDelayOption = getParaValue(params, m_paraDefsByKey["low_delay"]).value().toBool();
		opts.tcpKeepAliveOption = getParaValue(params, m_paraDefsByKey["keep_alive"]).value().toBool();
		return {new TcpSocketIO(opts)};
	}
}