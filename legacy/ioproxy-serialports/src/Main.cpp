#include <QSerialPortInfo>
#include <QtCore/QCoreApplication>
#include <ioproxy/Logging.hpp>
#include <ioproxy/Version.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("IOProxy SerialPorts");
	a.setApplicationVersion(IOPROXY_VERSION);
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://www.insanefactory.com");

	ioproxy::PrintAppInfoHeader();

	auto portInfos = QSerialPortInfo::availablePorts();
	if (portInfos.empty())
	{
		std::cout << "No ports found." << std::endl;
		return 0;
	}

	for (int i = 0; i < portInfos.size(); ++i)
	{
		const auto& portInfo = portInfos.at(i);
		std::cout << i << ": " << portInfo.systemLocation().toStdString() << " (" << portInfo.portName().toStdString() << ")" << std::endl;
	}
	return 0;
}