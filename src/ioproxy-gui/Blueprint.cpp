#include "Blueprint.hpp"

Blueprint::Blueprint(QObject* parent)
	: QObject(parent)
{}

Blueprint::~Blueprint()
{}

BlueprintItem* Blueprint::newItem()
{
	auto bpi = new BlueprintItem(this);
	_items.push_back(bpi);
	return bpi;
}
