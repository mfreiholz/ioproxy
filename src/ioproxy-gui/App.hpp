#pragma once
#include "Blueprint.hpp"
#include <QtCore/QObject>

class App : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString version MEMBER _version CONSTANT)

public:
	App(QObject* parent);
	~App() override;

	Q_INVOKABLE Blueprint* newBlueprint();
	Q_INVOKABLE Blueprint* openFromFile(const QString& filePath);

private:
	QString _version;
};
