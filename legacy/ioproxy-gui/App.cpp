#include "App.hpp"
#include "Blueprint.hpp"
#include "BlueprintItem.hpp"

App::App(QObject* parent)
	: QObject(parent)
	, _version("n/A")
{
	qRegisterMetaType<Blueprint*>();
	qRegisterMetaType<BlueprintItem*>();
}

App::~App()
{}

Blueprint* App::newBlueprint()
{
	return new Blueprint(this);
}

Blueprint* App::openFromFile(const QString& filePath)
{
	return new Blueprint(this);
}
