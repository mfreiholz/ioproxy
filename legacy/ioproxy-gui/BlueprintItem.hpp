#pragma once
#include <QtCore/QObject>

class BlueprintItem : public QObject
{
	Q_OBJECT

public:
	BlueprintItem(QObject* parent);
	~BlueprintItem() override = default;
};

Q_DECLARE_METATYPE(BlueprintItem*);
