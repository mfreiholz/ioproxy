#pragma once
#include "../IOFactoryBase.hpp"

class ByteGeneratorFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("bytes_generator");

	ByteGeneratorFactory();
	QString getID() const override;
	tl::expected<IOBase*, QString> create(const IOParameters& parameters);
};