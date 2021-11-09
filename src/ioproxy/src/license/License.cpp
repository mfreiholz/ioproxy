#include <QTextStream>
#include <ioproxy/license/License.hpp>

bool License::read(QString text)
{
	QTextStream ts(&text);
	QString line;

	// Read header.
	if (!ts.readLineInto(&line) || line.compare(QStringLiteral("License 1")) != 0)
		return false;

	// Read configuration statements.
	while (ts.readLineInto(&line))
	{
		if (line.isEmpty())
			continue;

		auto spacePos = line.indexOf(' ');
		if (spacePos < 0)
			continue;

		auto key = line.mid(0, spacePos);
		if (key.compare(QStringLiteral("allow")) == 0)
		{
			auto value = line.mid(spacePos + 1);
			if (value.compare("all") == 0)
			{
				m_allowAll = true;
			}
			else
			{
				m_allows.push_back(value);
			}
		}
	}
	return true;
}