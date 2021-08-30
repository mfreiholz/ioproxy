#include "Util.hpp"

namespace Util
{
	std::optional<QNetworkInterface> findNetworkInterfaceByIP(const QHostAddress& addr)
	{
		for (const QNetworkInterface& iface : QNetworkInterface::allInterfaces())
		{
			for (const QNetworkAddressEntry& entry : iface.addressEntries())
			{
				if (entry.ip().isEqual(addr, QHostAddress::StrictConversion))
				{
					return iface;
				}
			}
		}
		return {};
	}
}