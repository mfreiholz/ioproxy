#include "App.hpp"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

using namespace Qt::StringLiterals;

int main(int argc, char** argv)
{
	QGuiApplication qapp(argc, argv);
	qapp.setApplicationName("IOProxy GUI");
	qapp.setApplicationVersion("1.0.0");
	qapp.setOrganizationName("insaneFactory");
	qapp.setOrganizationDomain("https://insanefactory.com");
	qapp.setQuitOnLastWindowClosed(true);

	App appObj(nullptr);

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("AppObject", &appObj);

	const QUrl url(u"qrc:/qml/Main.qml"_s);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &qapp,
		[url](QObject* obj, const QUrl& objUrl) {
			if (!obj && url == objUrl)
				QCoreApplication::exit(-1);
		});
	engine.load(url);

	return qapp.exec();
}
