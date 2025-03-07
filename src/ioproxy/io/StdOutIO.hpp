#pragma once
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <iostream>

class StdOutIO : public IOBase
{
	Q_OBJECT

public:
	StdOutIO();
	StdOutIO(const StdOutIO&) = delete;
	StdOutIO& operator=(const StdOutIO&) = delete;
	~StdOutIO() override;
	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;
};

/*! ... */
class StdOutFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("stdout");

	StdOutFactory();
	QString getID() const override;
	QList<IOParaDef> getParameterDefinitions() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};