#pragma once
#include "Global.hpp"
#include "IOBase.hpp"
#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>
#include <cinttypes>

class TcpSocketIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QHostAddress remoteAddress;
		uint16_t remotePort;
		bool reconnect = false;
	};

	TcpSocketIO(QObject* parent = nullptr)
		: IOBase(parent)
	{}

	~TcpSocketIO() override
	{
		stop();
	}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start()
	{
		connectToRemote();
		emit ready();
	}

	void stop()
	{
		if (m_socket)
		{
			m_socket->disconnect(this);
			m_socket->close();
			m_socket.reset(nullptr);
		}
	}

	void writeData(const QByteArray& data) override
	{
		if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
		{
			HL_ERROR(LL, "Socket not connected yet");
			return;
		}
		m_socket->write(data);
	}

protected:
	void connectToRemote()
	{
		stop();
		m_socket = std::make_unique<QTcpSocket>();
		QObject::connect(m_socket.get(), &QTcpSocket::connected, this, &TcpSocketIO::onSocketConnected);
		QObject::connect(m_socket.get(), &QTcpSocket::disconnected, this, &TcpSocketIO::onSocketDisconnected);
		QObject::connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &TcpSocketIO::onSocketErrorOccured);
		QObject::connect(m_socket.get(), &QTcpSocket::readyRead, this, &TcpSocketIO::onSocketReadyRead);
		m_socket->connectToHost(m_options.remoteAddress, m_options.remotePort);
	}

private slots:
	void onSocketConnected()
	{
		HL_INFO(LL, "onSocketConnected");
	}

	void onSocketDisconnected()
	{
		HL_INFO(LL, "onSocketDisconnected");
	}

	void onSocketErrorOccured(QAbstractSocket::SocketError)
	{
		HL_INFO(LL, "onSocketErrorOccured");
		if (m_options.reconnect)
		{
			reconnect();
		}
	}

	void onSocketReadyRead()
	{
		while (m_socket->bytesAvailable() > 0)
		{
			auto data = m_socket->read(m_socket->bytesAvailable());
			emit newData(data);
		}
	}

	void reconnect()
	{
		HL_INFO(LL, "Initiate auto reconnect");
		QTimer::singleShot(1000, this, &TcpSocketIO::connectToRemote);
	}

private:
	Options m_options;
	std::unique_ptr<QTcpSocket> m_socket;
};