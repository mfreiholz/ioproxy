#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>

// 576=Safe datagram size (minus header size).
const int UDP_SAFE_PAYLOAD_SIZE = 508;

int main(int argc, char** argv)
{
	QCoreApplication a(argc, argv);
	const auto host = QHostAddress(a.arguments().at(1));
	const auto port = a.arguments().at(2).toInt();

	QUdpSocket sock;

	QTimer timer;
	timer.setSingleShot(true);
	timer.setInterval(0);

	QObject::connect(&timer, &QTimer::timeout,
		[&]() {
			timer.stop();
			QByteArray ba;
			ba.resize(UDP_SAFE_PAYLOAD_SIZE);
			ba.fill('1', ba.size());
			sock.writeDatagram(ba, host, port);
			timer.start();
		});

	timer.start();
	return a.exec();
}