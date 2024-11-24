#include "StdOutIO.hpp"
#include <humblelogging/humblelogging.h>

HUMBLE_LOGGER(L, "ioproxy.stdout");

namespace ioproxy
{
	///////////////////////////////////////////////////////////////////
	// IO
	///////////////////////////////////////////////////////////////////

	StdOutIO::StdOutIO()
		: IOBase()
	{}

	StdOutIO::~StdOutIO()
	{}

	void StdOutIO::start()
	{
		Q_EMIT started();
	}

	void StdOutIO::stop()
	{}

	void StdOutIO::writeData(const DataPack& data)
	{
		QString str(data.bytes);
		auto s = str.toStdString();
		std::cout << s;
		m_statistic.bytesWritten += str.size();
	}

	///////////////////////////////////////////////////////////////////
	// Factory
	///////////////////////////////////////////////////////////////////

	StdOutFactory::StdOutFactory()
		: BuiltInIOFactoryBase()
	{}

	QString StdOutFactory::getID() const
	{
		return StdOutFactory::ID;
	}

	QList<IOParameterDefinition> StdOutFactory::getParameterDefinitions() const
	{
		return m_paraDefs;
	}

	tl::expected<IOBase*, QString> StdOutFactory::create(const IOParameters&)
	{
		return {new StdOutIO()};
	}
}