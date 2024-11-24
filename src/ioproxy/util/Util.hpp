#pragma once
#include <QHostAddress>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <optional>

namespace Util
{
	std::optional<QNetworkInterface> findNetworkInterfaceByIP(const QHostAddress& addr);
}