#include "ByteGeneratorFactory.hpp"
#include "ByteGeneratorIO.hpp"

ByteGeneratorFactory::ByteGeneratorFactory()
	: BuiltInIOFactoryBase()
{
	IOParaDef def;

	def.key = "rate";
	def.description = "Number of bytes to generate per second.";
	def.valueDataType = IOParaDef::UINT64;
	def.required = false;
	def.multi = false;
	def.defaultValue = 1024 * 1024 * 5;
	addParaDef(def);

	def.key = "packet_size";
	def.description = "Size of a single packet.";
	def.valueDataType = IOParaDef::UINT64;
	def.required = false;
	def.multi = false;
	def.defaultValue = 1024 * 4;
	addParaDef(def);
}

QString ByteGeneratorFactory::getID() const
{
	return ID;
}

tl::expected<IOBase*, QString> ByteGeneratorFactory::create(const IOParameters& params)
{
	auto eParaCheck = checkRequiredParameters(params);
	if (!eParaCheck)
		return tl::make_unexpected(eParaCheck.error());

	ByteGeneratorIO::Options opts;
	opts.bytesPerSecond = this->getParaValue(params, m_paraDefsByKey["rate"]).value().toULongLong();
	opts.packetSize = this->getParaValue(params, m_paraDefsByKey["packet_size"]).value().toULongLong();

	return {new ByteGeneratorIO(opts, nullptr)};
}