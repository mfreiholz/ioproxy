#pragma once
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtGlobal>
#include <cinttypes>
#include <memory>

class TcpServerIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QHostAddress bindAddress = QHostAddress::Any;
		uint16_t bindPort = 0;

		// Maximum number of parallel connections
		// It doesn't make sense to have more than one connection sending data to this server.
		// Data from "newData()" signal would get mixed up.
		int maxConnections = 1;

		// If `true` all incoming data gets broadcasted to all other clients.
		// @todo Note: What about applying filter?
		bool broadcast = false;

		qint64 readBufferSize = 32 * 1024; // 32 MB.
	};

	TcpServerIO(const Options& options);
	~TcpServerIO() override;

public Q_SLOTS:
	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

private Q_SLOTS:
	void writeSocket(QTcpSocket* sock, const DataPack& data);
	void onServerAcceptError(QAbstractSocket::SocketError socketError);
	void onServerNewConnection();
	void onSocketReadyRead();
	void onSocketErrorOccured(QAbstractSocket::SocketError);
	void onSocketDisconnected();

Q_SIGNALS:
	void bindAddressChanged();
	void bindPortChanged();

private:
	Options m_options;
	QTcpServer m_server;
	QList<QTcpSocket*> m_sockets;
};

///////////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////////

class TcpServerIOFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("tcpserver");

	TcpServerIOFactory();
	QString getID() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};