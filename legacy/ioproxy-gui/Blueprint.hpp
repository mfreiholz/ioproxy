#pragma once
#include "BlueprintItem.hpp"
#include <QtCore/QObject>

class Blueprint : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QList<BlueprintItem*> items MEMBER _items)

public:
	Blueprint(QObject* parent = nullptr);
	~Blueprint() override;

	Q_INVOKABLE BlueprintItem* newItem();

private:
	QList<BlueprintItem*> _items;

signals:
};

Q_DECLARE_METATYPE(Blueprint*);
