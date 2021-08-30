#pragma once
#include "Global.hpp"
#include "IOBase.hpp"
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <cinttypes>
#include <memory>

class TcpServerIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QHostAddress bindAddress;
		uint16_t bindPort;

		// Maximum number of parallel connections
		// It doesn't make sense to have more than one connection sending data to this server.
		// Data from "newData()" signal would get mixed up.
		int maxConnections = 1;
	};

	TcpServerIO()
		: IOBase()
	{
		QObject::connect(&m_server, &QTcpServer::acceptError, this, &TcpServerIO::onServerAcceptError);
		QObject::connect(&m_server, &QTcpServer::newConnection, this, &TcpServerIO::onServerNewConnection);
	}

	~TcpServerIO() override
	{
		stop();
	}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start() override
	{
		QHostAddress bindAddress(QHostAddress::Any);
		if (!m_options.bindAddress.isNull())
		{
			bindAddress = m_options.bindAddress;
		}

		if (!m_server.listen(bindAddress, m_options.bindPort))
		{
			emit errorOccured(QString("Can not listen: %1").arg(m_server.errorString()));
			return;
		}
		emit ready();
	}

	void stop() override
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

	void writeData(const QByteArray& data) override
	{
		for (auto sock : m_sockets)
		{
			sock->write(data);
		}
	}

private slots:
	void onServerAcceptError(QAbstractSocket::SocketError)
	{
		HL_ERROR(LL, QString("Can not accept socket: %1").arg(m_server.errorString()).toStdString());
	}

	void onServerNewConnection()
	{
		auto sock = m_server.nextPendingConnection();
		if (!sock)
		{
			HL_ERROR(LL, "No pending connections");
			return;
		}

		// do not allow more than "maxConnections" connections
		if (m_sockets.size() >= m_options.maxConnections)
		{
			sock->abort();
			delete sock;
			sock = nullptr;
			return;
		}

		// use socket
		QObject::connect(sock, &QTcpSocket::readyRead, this, &TcpServerIO::onSocketReadyRead);
		QObject::connect(sock, &QTcpSocket::errorOccurred, this, &TcpServerIO::onSocketErrorOccured);
		QObject::connect(sock, &QTcpSocket::disconnected, this, &TcpServerIO::onSocketDisconnected);
		m_sockets.push_back(sock);
	}

	void onSocketReadyRead()
	{
		auto sock = qobject_cast<QTcpSocket*>(sender());
		if (sock->bytesAvailable())
		{
			auto data = sock->read(sock->bytesAvailable());
			emit newData(data);
		}
	}

	void onSocketErrorOccured(QAbstractSocket::SocketError)
	{
		auto sock = qobject_cast<QTcpSocket*>(sender());
		HL_ERROR(LL, "TcpServerIO::onSocketErrorOccured");
	}

	void onSocketDisconnected()
	{
		auto sock = qobject_cast<QTcpSocket*>(sender());
		HL_ERROR(LL, "TcpServerIO::onSocketDisconnected");
		m_sockets.removeOne(sock);
		sock->deleteLater();
	}

private:
	Options m_options;
	QTcpServer m_server;
	QList<QTcpSocket*> m_sockets;
};