#pragma once

#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/Time.hpp>

#include <SFML/Network.hpp>
#include <vector>

#include <Common.hpp>
#include <Endpoint.hpp>

#include "ServerSocket.hpp"

class Server
{
public:
	Server();

	bool Start(int argc, char** argv);
	void Stop();

	bool Run();
	bool IsRunning() const;

private:
	void HandleIncomingPackets();
	en::U32 GetIndexFromEndpoint(const en::Endpoint& endpoint) const;

private:
	void SendToAllPlayers(sf::Packet& packet);

	void SendPingPacket(const en::Endpoint& endpoint);
	void SendPongPacket(const en::Endpoint& endpoint);
	void SendConnectionAcceptedPacket(const en::Endpoint& endpoint, en::U32 playerIndex, const en::Time& timePerReaction);
	void SendConnectionRejectedPacket(const en::Endpoint& endpoint);
	void SendClientJoinedPacket(en::U32 playerIndex);
	void SendClientLeftPacket(en::U32 playerIndex);
	void SendServerStopPacket();
	void SendGameInfo();

private:  
	ServerSocket mSocket;
	bool mRunning;

	struct Player
	{
		en::Endpoint endpoint;
	};
	std::vector<Player> mPlayers;
};
