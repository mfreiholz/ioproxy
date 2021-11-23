#include "CmdLineConfig.hpp"
#include "IOProxyConsoleApplication.hpp"
#include <QDir>
#include <QMetaType>
#include <QSerialPortInfo>
#include <QtCore/QCoreApplication>
#include <humblelogging/humblelogging.h>
#include <ioproxy/App.hpp>
#include <ioproxy/AppContext.hpp>
#include <ioproxy/CommandLineInitializer.hpp>
#include <ioproxy/Engine.hpp>
#include <ioproxy/IOBase.hpp>
#include <ioproxy/license/License.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

HUMBLE_LOGGER(L, "ioproxy-cli");

void initConsole()
{
#ifdef WIN32
	AllocConsole();
#endif
}

void initLogging()
{
	using namespace humble::logging;
	Factory& fac = Factory::getInstance();
	fac.setDefaultFormatter(new PatternFormatter("%date\t%lls\t%m\n"));
	fac.setConfiguration(new SimpleConfiguration(LogLevel::All));
	fac.registerAppender(new ConsoleAppender());
}

void initLicense()
{
	// Read contents from license file.
	const QString licenseFileName("license.lic");
	QDir dir(QCoreApplication::applicationDirPath());
	if (!dir.exists(licenseFileName))
		return;

	QFile f(dir.absoluteFilePath(licenseFileName));
	if (!f.open(QIODevice::ReadOnly))
		return;

	auto encryptedBytes = f.readAll();
	f.close();

	// Decrypt contents.
	auto decryptedBytes = encryptedBytes;

	QString txt;
	txt.append("License 1\n");
	txt.append("creator Manuel Freiholz\n");
	txt.append("allow all\n");
	txt.append("allow core\n");
	txt.append("option io.tcpserver.maxclients=1\n");

	// Create license from plain content.
	License lic;
	lic.read(QString(decryptedBytes));
}

void printSerialPorts()
{
	auto portInfos = QSerialPortInfo::availablePorts();
	for (auto portInfo : portInfos)
	{
		std::cout << portInfo.portName().toStdString() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("IOProxy");
	a.setApplicationVersion("1.0.0");
	a.setOrganizationName("insaneFactory");
	a.setOrganizationDomain("https://insanefactory.com");

	initLogging();
	ioproxy::Engine::RegisterMetaTypes();

	HL_INFO(L, QString("%1 - %2").arg(a.applicationName()).arg(a.applicationVersion()).toStdString());

	if (a.arguments().size() >= 2 && a.arguments()[1] == "serialports")
	{
		printSerialPorts();
		return 0;
	}

	IOProxyConsoleApplication ioApp(nullptr);
	QObject::connect(&ioApp, &IOProxyConsoleApplication::finished, &a, &QCoreApplication::quit);
	QObject::connect(&ioApp, &IOProxyConsoleApplication::finishedWithSignal, &a, [&]() {
		HL_INFO(L, "Quit on user request.");
		a.quit();
	});
	QObject::connect(&ioApp, &IOProxyConsoleApplication::failed, [&](int code, const QString& errorMessage) {
		HL_INFO(L, QString("Quit on error (%1) %2").arg(QString::number(code)).arg(errorMessage).toStdString());
		a.quit();
	});
	QMetaObject::invokeMethod(&ioApp, &IOProxyConsoleApplication::startup, Qt::QueuedConnection);

	return a.exec();
}