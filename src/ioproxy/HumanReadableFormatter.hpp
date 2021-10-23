#pragma once
#include <QString>
#include <QtGlobal>

class HumanReadableFormatter
{
public:
	static QString fileSize(qint64 size, int precision = 2);
};