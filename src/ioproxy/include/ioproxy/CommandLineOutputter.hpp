#pragma once
#include <QObject>
#include <memory>

class CommandLineOutputter : public QObject
{
	Q_OBJECT

public:
	CommandLineOutputter();

private:
	class Private;
	std::unique_ptr<Private> d;
};