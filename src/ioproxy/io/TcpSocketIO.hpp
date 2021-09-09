#pragma once
#include "../Global.hpp"
#include "../IOBase.hpp"
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

	TcpSocketIO()
		: IOBase()
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
		emit started();
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
			return;
		}

		auto written = m_socket->write(data);
		if (written != data.size())
		{
			emit errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
								  .arg(written)
								  .arg(data.size())
								  .arg(m_socket->peerAddress().toString())
								  .arg(m_socket->peerPort()));
		}
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
		HL_INFO(LL, QString("Connected to remote %1:%2").arg(m_options.remoteAddress.toString()).arg(m_options.remotePort).toStdString());
	}

	void onSocketDisconnected()
	{
	}

	void onSocketErrorOccured(QAbstractSocket::SocketError)
	{
		emit errorOccured(QString("Error for connection to %1:%2 (%3)")
							  .arg(m_options.remoteAddress.toString())
							  .arg(m_options.remotePort)
							  .arg(m_socket->errorString()));
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