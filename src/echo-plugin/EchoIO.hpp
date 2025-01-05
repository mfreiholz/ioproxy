#pragma once
#include "ioproxy/io/IOBase.hpp"
#include "ioproxy/io/IOFactoryBase.hpp"

class EchoIO : public IOBase
{
	Q_OBJECT

public:
	EchoIO();
	EchoIO(const EchoIO&) = delete;
	EchoIO& operator=(const EchoIO&) = delete;
	~EchoIO() override;
	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;
};


/*! ... */
class EchoIOFactory : public IOFactoryBase
{
public:
	inline static const QString ID = QString("echo");

	EchoIOFactory();
	QString getID() const override;
	QList<IOParaDef> getParameterDefinitions() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);

private:
	QList<IOParaDef> m_paraDefs;
};