#include "ioproxy/io/TcpServerIO.hpp"
#include <humblelogging/humblelogging.h>

HUMBLE_LOGGER(LL, "ioproxy.tcpserver");

///////////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////////

TcpServerIO::TcpServerIO(const Options& options)
	: IOBase()
	, m_options(options)
{
	QObject::connect(&m_server, &QTcpServer::acceptError, this, &TcpServerIO::onServerAcceptError);
	QObject::connect(&m_server, &QTcpServer::newConnection, this, &TcpServerIO::onServerNewConnection);
}

TcpServerIO::~TcpServerIO()
{
	stop();
}

void TcpServerIO::start()
{
	QHostAddress bindAddress(QHostAddress::Any);
	if (!m_options.bindAddress.isNull())
	{
		bindAddress = m_options.bindAddress;
	}
	if (!m_server.listen(bindAddress, m_options.bindPort))
	{
		Q_EMIT startupErrorOccured(QString("Can not listen: %1").arg(m_server.errorString()));
		return;
	}
	Q_EMIT started();
}

void TcpServerIO::stop()
{
	m_server.close();
	for (auto sock : m_sockets)
	{
		sock->abort();
		delete sock;
		sock = nullptr;
	}
	m_sockets.clear();
}

void TcpServerIO::writeData(const DataPack& data)
{
	for (auto sock : m_sockets)
	{
		writeSocket(sock, data);
	}
}

void TcpServerIO::writeSocket(QTcpSocket* sock, const DataPack& data)
{
	auto written = sock->write(data.bytes);
	if (written != data.bytes.size())
	{
		Q_EMIT errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
								.arg(written)
								.arg(data.bytes.size())
								.arg(sock->peerAddress().toString())
								.arg(sock->peerPort()));
		return;
	}
	m_statistic.bytesWritten += written;
}

void TcpServerIO::onServerAcceptError(QAbstractSocket::SocketError socketError)
{
	Q_EMIT errorOccured(QString("Can not accept socket. %1 %2").arg(socketError).arg(m_server.errorString()));
}

void TcpServerIO::onServerNewConnection()
{
	auto sock = m_server.nextPendingConnection();
	if (!sock)
		return;

	// do not allow more than "maxConnections" connections
	if (m_sockets.size() >= m_options.maxConnections)
	{
		emit errorOccured(QString("Refuse incoming connection from %1:%2 (max-connections=%3)").arg(sock->peerAddress().toString()).arg(sock->peerPort()).arg(m_options.maxConnections));
		sock->abort();
		delete sock;
		sock = nullptr;
		return;
	}

	sock->setReadBufferSize(m_options.readBufferSize);

	// use socket
	QObject::connect(sock, &QTcpSocket::readyRead, this, &TcpServerIO::onSocketReadyRead);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
	QObject::connect(sock, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpServerIO::onSocketErrorOccured);
#else
	QObject::connect(sock, &QTcpSocket::errorOccurred, this, &TcpServerIO::onSocketErrorOccured);
#endif
	QObject::connect(sock, &QTcpSocket::disconnected, this, &TcpServerIO::onSocketDisconnected);
	m_sockets.push_back(sock);
}

void TcpServerIO::onSocketReadyRead()
{
	auto sock = qobject_cast<QTcpSocket*>(sender());
	if (sock->bytesAvailable())
	{
		DataPack data(sock->read(sock->bytesAvailable()));
		if (m_options.broadcast)
		{
			for (int i = 0; i < m_sockets.size(); ++i)
			{
				if (m_sockets[i] == sock)
					continue;
				writeSocket(m_sockets[i], data);
			}
		}
		m_statistic.bytesRead += data.bytes.size();
		Q_EMIT newData(data);
	}
}

void TcpServerIO::onSocketErrorOccured(QAbstractSocket::SocketError)
{
	auto sock = qobject_cast<QTcpSocket*>(sender());
	Q_EMIT errorOccured(QString("Error for connection from %1:%2 (%3)")
							.arg(sock->peerAddress().toString())
							.arg(sock->peerPort())
							.arg(sock->errorString()));
}

void TcpServerIO::onSocketDisconnected()
{
	auto sock = qobject_cast<QTcpSocket*>(sender());
	m_sockets.removeOne(sock);
	sock->deleteLater();
}

///////////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////////

TcpServerIOFactory::TcpServerIOFactory()
	: BuiltInIOFactoryBase()
{
	IOParaDef def;

	def.key = "bind_address";
	def.description = "";
	def.valueDataType = IOParaDef::STRING;
	def.required = false;
	def.multi = false;
	def.defaultValue = "0.0.0.0";
	addParaDef(def);

	def.key = "bind_port";
	def.description = "";
	def.valueDataType = IOParaDef::UINT16;
	def.required = true;
	def.multi = false;
	addParaDef(def);

	def.key = "max_connections";
	def.description = "";
	def.valueDataType = IOParaDef::INT32;
	def.required = false;
	def.defaultValue = 1;
	addParaDef(def);

	def.key = "broadcast";
	def.description = "";
	def.valueDataType = IOParaDef::BOOLEAN;
	def.required = false;
	def.defaultValue = false;
	addParaDef(def);

	def.key = "read_buffer_size";
	def.description = "";
	def.valueDataType = IOParaDef::INT64;
	def.required = false;
	def.defaultValue = 32 * 1024;
	addParaDef(def);
}

QString TcpServerIOFactory::getID() const
{
	return TcpServerIOFactory::ID;
}

tl::expected<IOBase*, QString> TcpServerIOFactory::create(const IOParameters& params)
{
	auto eParaCheck = checkRequiredParameters(params);
	if (!eParaCheck)
		return tl::make_unexpected(eParaCheck.error());

	TcpServerIO::Options opts;
	opts.bindAddress = QHostAddress(getParaValue(params, m_paraDefsByKey["bind_address"]).value().toString());
	opts.bindPort = getParaValue(params, m_paraDefsByKey["bind_port"]).value().toUInt();
	opts.maxConnections = getParaValue(params, m_paraDefsByKey["max_connections"]).value().toUInt();
	opts.broadcast = getParaValue(params, m_paraDefsByKey["broadcast"]).value().toBool();
	opts.readBufferSize = getParaValue(params, m_paraDefsByKey["read_buffer_size"]).value().toLongLong();
	return {new TcpServerIO(opts)};
}