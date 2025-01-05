#include <QTcpSocket>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication a(argc, argv);
	const QString host = a.arguments().at(1);
	const auto port = a.arguments().at(2).toInt();

	QTcpSocket sock;
	sock.connectToHost(host, port);
	if (!sock.waitForConnected())
		return 1;

	int index = -1;
	if (a.arguments().indexOf("--low-delay") >= 0)
	{
		sock.setSocketOption(QAbstractSocket::LowDelayOption, 1);
	}
	if ((index = a.arguments().indexOf("--send-buffer-size")) >= 0)
	{
		auto bsize = a.arguments().at(index + 1).toInt();
		sock.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, bsize);
	}

	// Should be bigger than the OS write buffer size (16KB).
	QByteArray ba;
	ba.resize(1024 * 32);
	for (; sock.state() == QAbstractSocket::ConnectedState;)
	{
		sock.write(ba);
		sock.waitForBytesWritten();
	}
	sock.disconnectFromHost();
	if (!sock.waitForDisconnected())
		return 2;

	return 0;
}