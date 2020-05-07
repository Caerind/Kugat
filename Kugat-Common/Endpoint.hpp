#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <SFML/Network/IpAddress.hpp>

namespace en
{

class Endpoint
{
public:
	Endpoint();
	Endpoint(const sf::IpAddress& address, en::U16 port);

	const sf::IpAddress& GetAddress() const;
	en::U16 GetPort() const;

	void SetAddress(const sf::IpAddress& address);
	void SetPort(en::U16 port);

	bool operator==(const Endpoint& other) const;
	bool operator!=(const Endpoint& other) const;

private:
	sf::IpAddress mAddress;
	en::U16 mPort;
};

} // namespace en