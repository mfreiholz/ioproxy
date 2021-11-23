#include <QTcpSocket>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication a(argc, argv);

	QTcpSocket sock;
	sock.connectToHost("127.0.0.1", 12345);
	if (!sock.waitForConnected())
		return 1;

	if (a.arguments().indexOf("--low-delay") >= 0)
		sock.setSocketOption(QAbstractSocket::LowDelayOption, 1);

	//sock.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 1024 * 4);

	// Should be bigger than the OS write buffer size (16KB).
	QByteArray ba;
	ba.resize(1024 * 32);

	for (;;)
	{
		sock.write(ba);
		sock.waitForBytesWritten();
	}

	sock.disconnectFromHost();
	if (!sock.waitForDisconnected())
		return 2;

	return 0;
}