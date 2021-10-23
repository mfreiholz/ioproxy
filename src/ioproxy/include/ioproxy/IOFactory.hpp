#pragma once
#include <QFlags>
#include <QString>
#include <cinttypes>

class IOFactory
{
public:
	enum class Capability
	{
		None = 0x0000,
		Input = 0x0001,
		Output = 0x0002
	};
	Q_DECLARE_FLAGS(Capabilities, Capability)

	IOFactory(const QString& id, const QString& title)
		: m_id(id)
		, m_title(title)
	{}

	virtual ~IOFactory() = default;

	QString id() const
	{
		return m_id;
	}

	QString title() const
	{
		return m_title;
	}

	QFlags<Capabilities> capabilities() const
	{
		return m_caps;
	}

private:
	QString m_id;
	QString m_title;
	QFlags<Capabilities> m_caps;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(IOFactory::Capabilities)