#include <ioproxy/HumanReadableFormatter.hpp>

QString HumanReadableFormatter::fileSize(qint64 size, int precision)
{
	QString abbrevation;
	double divider = 0;

	if (size > 1152921504606846976)
	{
		abbrevation = "EB";
		divider = 1152921504606846976;
	}
	else if (size > 1125899906842624)
	{
		divider = 1125899906842624;
		abbrevation = "PB";
	}
	else if (size > 1099511627776)
	{
		divider = 1099511627776;
		abbrevation = "TB";
	}
	else if (size > 1073741824)
	{
		divider = 1073741824;
		abbrevation = "GB";
	}
	else if (size > 1048576)
	{
		divider = 1048576;
		abbrevation = "MB";
	}
	else if (size > 1024)
	{
		divider = 1024;
		abbrevation = "KB";
	}
	else
	{
		divider = 1;
		abbrevation = "Bytes";
	}

	double d = (static_cast<double>(size) / divider);
	return QString("%1 %2").arg(d, 0, 'f', divider > 1 ? precision : 0).arg(abbrevation);
}
