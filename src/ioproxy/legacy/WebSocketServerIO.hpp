#pragma once
#include "../IOBase.hpp"
#include <QWebSocket>
#include <QWebSocketServer>
#include <memory>

class WebSocketServerIO : public IOBase
{
	Q_OBJECT

public:
	enum class DataMode
	{
		BinaryMode,
		TextMode
	};

	enum class DataHandling
	{
		//ByFrame, // Handles all incoming data by single frames.
		ByMessage // Handles all incoming data by complete messages.
	};

	struct Options
	{
		QHostAddress bindAddress = QHostAddress::Any;
		quint16 bindPort = 0;

		// Maximum number of parallel connections
		// It doesn't make sense to have more than one connection sending data to this server.
		// Data from "newData()" signal would get mixed up.
		int maxConnections = 1;

		// If `true` all incoming data gets broadcasted to all other clients.
		// @todo Note: What about applying filter?
		bool broadcast = false;

		DataMode writeDataMode = DataMode::BinaryMode;
		DataHandling dataHandling = DataHandling::ByMessage;
	};

	WebSocketServerIO()
		: IOBase()
		, m_server("ioproxy-websocket-server", QWebSocketServer::SslMode::NonSecureMode)
	{
		QObject::connect(&m_server, &QWebSocketServer::acceptError, this, &WebSocketServerIO::onServerAcceptError);
		QObject::connect(&m_server, &QWebSocketServer::newConnection, this, &WebSocketServerIO::onServerNewConnection);
	}

	~WebSocketServerIO() override
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
			emit startupErrorOccured(QString("Can not listen: %1").arg(m_server.errorString()));
			return;
		}
		emit started();
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

	void writeData(const DataPack& data) override
	{
		for (auto sock : m_sockets)
		{
			writeSocket(sock, data);
		}
	}

private slots:
	void writeSocket(QWebSocket* sock, const DataPack& data)
	{
		qint64 written = 0;
		switch (m_options.writeDataMode)
		{
			case DataMode::BinaryMode:
				written = sock->sendBinaryMessage(data.bytes);
				break;
			case DataMode::TextMode:
				written = sock->sendTextMessage(QString(data.bytes));
				break;
		}
		if (written != data.bytes.size())
		{
			emit errorOccured(QString("writtenBytes (%1) != data.size (%2); remote=%3:%4")
								  .arg(written)
								  .arg(data.bytes.size())
								  .arg(sock->peerAddress().toString())
								  .arg(sock->peerPort()));
			return;
		}
		m_statistic.bytesWritten += written;
	}

	void onServerAcceptError(QAbstractSocket::SocketError socketError)
	{
		emit errorOccured(QString("Can not accept socket. %1 %2").arg(socketError).arg(m_server.errorString()));
	}

	void onServerNewConnection()
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

		// use socket
		QObject::connect(sock, &QWebSocket::binaryMessageReceived, this, &WebSocketServerIO::onBinaryMessageReceived);
		QObject::connect(sock, &QWebSocket::textMessageReceived, this, &WebSocketServerIO::onTextMessageReceived);
		QObject::connect(sock, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &WebSocketServerIO::onSocketError);
		QObject::connect(sock, &QWebSocket::disconnected, this, &WebSocketServerIO::onSocketDisconnected);
		m_sockets.push_back(sock);
	}

	//void onBinaryFrameReceived(const QByteArray& frame, bool isLastFrame)
	//{
	//	DataPack dp;
	//	dp.bytes = frame;
	//	dp.fixedSize = frame.size();
	//	dp.parameters.insert("isLastFrame", isLastFrame);

	//	m_statistic.bytesRead += frame.size();
	//	emit newData(dp);
	//}

	void onBinaryMessageReceived(const QByteArray& message)
	{
		m_statistic.bytesRead += message.size();
		emit newData(message);
	}

	void onPong(quint64 elapsedTime, const QByteArray& payload)
	{}

	//void onTextFrameReceived(const QString& frame, bool isLastFrame)
	//{
	//	DataPack dp;
	//	dp.bytes = frame.toUtf8();
	//	dp.fixedSize = dp.bytes.size();
	//	dp.parameters.insert("isLastFrame", isLastFrame);

	//	m_statistic.bytesRead += frame.size();
	//	emit newData(dp);
	//}

	void onTextMessageReceived(const QString& message)
	{
		m_statistic.bytesRead += message.size();
		emit newData(message.toUtf8());
	}

	void onSocketError(QAbstractSocket::SocketError error)
	{
		auto sock = qobject_cast<QWebSocket*>(sender());
		emit errorOccured(QString("Error for connection from %1:%2 (%3)")
							  .arg(sock->peerAddress().toString())
							  .arg(sock->peerPort())
							  .arg(sock->errorString()));
	}

	void onSocketDisconnected()
	{
		auto sock = qobject_cast<QWebSocket*>(sender());
		m_sockets.removeOne(sock);
		sock->deleteLater();
	}

private:
	Options m_options;
	QWebSocketServer m_server;
	QList<QWebSocket*> m_sockets;
};