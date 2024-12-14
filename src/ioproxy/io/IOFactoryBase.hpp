#pragma once
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QVariant>
#include <tl/expected.hpp>
class IOBase;


/*! Definition of a single parameter of a IOBase implementation. */
struct IOParaDef
{
public:
	enum DataType
	{
		INT8,
		INT16,
		INT32,
		INT64,
		UINT8,
		UINT16,
		UINT32,
		UINT64,
		FLOAT32,
		DOUBLE64,
		BOOLEAN,
		STRING,
	};

	// Key of the parameter (e.g. "bind_port").
	QString key;

	// Description about the parameter itself.
	// What is it used for? Gives detailed description to the user.
	QString description;

	// Data type of the parameter's value.
	DataType valueDataType = DataType::STRING;

	// Indicates whether this parameter is required in order to use the IO implementation.
	bool required = false;

	// Indicates whether this parameter can/may occur multiple times.
	bool multi = false;

	// The default value.
	QVariant defaultValue;
};
//using IOParaDef = IOParameterDefinition;


/*!
	Provides information about a specific IOBase implementation and
	how to use/create it.
*/
class IOFactoryBase
{
public:
	using IOParams = QMultiMap<QString, QVariant>;
	using IOParameters = IOParams;

	virtual QString getID() const = 0;
	virtual QList<IOParaDef> getParameterDefinitions() const = 0;
	virtual tl::expected<IOBase*, QString> create(const IOParams& parameters) = 0;
};

/*!
	Move to separate file when we need this as SDK.
	We use this for all builtin types to have some additional helper functions,
	especially for parameter validation.
*/
class BuiltInIOFactoryBase : public IOFactoryBase
{
public:
	QList<IOParaDef> getParameterDefinitions() const override { return m_paraDefs; }

protected:
	void addParaDef(const IOParaDef& paraDef);
	tl::expected<void, QString> checkRequiredParameters(const IOParams& parameters) const;
	tl::expected<QVariant, QString> getParaValue(const IOParams& params, const IOParaDef& def, uint32_t index = 0) const;

protected:
	QList<IOParaDef> m_paraDefs;
	QMap<QString, IOParaDef> m_paraDefsByKey;
};