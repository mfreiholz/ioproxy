#include "IOFactoryBase.hpp"

void BuiltInIOFactoryBase::addParaDef(const IOParaDef& paraDef)
{
	m_paraDefs.push_back(paraDef);
	m_paraDefsByKey.insert(paraDef.key, paraDef);
}

tl::expected<void, QString> BuiltInIOFactoryBase::checkRequiredParameters(const IOParams& parameters) const
{
	QList<IOParaDef> defs = this->getParameterDefinitions();
	for (auto it = defs.constBegin(); it != defs.constEnd(); it++)
	{
		if (it->required && parameters.constFind(it->key) == parameters.constEnd())
		{
			return tl::make_unexpected(QString("Missing required parameter \"%1\".").arg(it->key));
		}
	}
	return {};
}

tl::expected<QVariant, QString> BuiltInIOFactoryBase::getParaValue(const IOParams& params, const IOParaDef& def, uint32_t index) const
{
	if (!params.contains(def.key))
	{
		if (def.required)
		{
			return tl::make_unexpected(QString("Missing REQUIRED parameter \"%1\".").arg(def.key));
		}
		return def.defaultValue;
	}

	QVariantList values = params.values(def.key);
	if (static_cast<int32_t>(values.size()) - 1 < index)
	{
		return tl::make_unexpected(QString("Missing parameter \"%1\" with index \"%2\".").arg(def.key).arg(index));
	}
	return values.at(index);
}