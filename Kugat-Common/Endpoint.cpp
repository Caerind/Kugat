#include "Endpoint.hpp"

namespace en
{

Endpoint::Endpoint()
	: mAddress()
	, mPort(0)
{
}

Endpoint::Endpoint(const sf::IpAddress& address, en::U16 port)
	: mAddress(address)
	, mPort(port)
{
}

const sf::IpAddress& Endpoint::GetAddress() const
{
	return mAddress;
}

en::U16 Endpoint::GetPort() const
{
	return mPort;
}

void Endpoint::SetAddress(const sf::IpAddress& address)
{
	mAddress = address;
}

void Endpoint::SetPort(en::U16 port)
{
	mPort = port;
}

bool Endpoint::operator==(const Endpoint& other) const
{
	return mPort == other.mPort && mAddress == other.mAddress;
}

bool Endpoint::operator!=(const Endpoint& other) const
{
	return !operator==(other);
}

} // namespace en