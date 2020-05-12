#include "Server.hpp"

#include <Enlivengine/Application/PathManager.hpp>
#include "GameData.hpp"

static const constexpr en::U16 defaultPort = 3458;
static const constexpr en::U32 defaultMaxPlayers = 4;

Server::Server()
	: mSocket()
	, mRunning(false)
{
}

bool Server::Start(int argc, char** argv)
{
#ifdef ENLIVE_ENABLE_LOG
	en::LogManager::GetInstance().Initialize();
#endif

	if (argc >= 1)
	{
		en::PathManager::GetInstance().SetExecutablePath(argv[0]);
		bool success = GameData::LoadFromFile(en::PathManager::GetInstance().GetAssetsPath() + "gamestats.xml");
		if (!success)
		{
			success = GameData::LoadFromFile("gamestats.xml");
			if (!success)
			{
				return false;
			}
		}
	}
	else
	{
		if (!GameData::LoadFromFile("gamestats.xml"))
		{
			return false;
		}
	}

	if (!mSocket.Start((argc >= 2) ? static_cast<en::U16>(std::atoi(argv[1])) : defaultPort))
	{
		return false;
	}

	mRunning = true;

	return true;
}

void Server::Stop()
{
	mRunning = false;

	mSocket.SetBlocking(true);
	sf::Packet stoppingPacket;
	stoppingPacket << static_cast<en::U8>(ServerPacketID::Stopping);
	SendToAllPlayers(stoppingPacket);

	mSocket.Stop();
}

bool Server::Run()
{
	while (IsRunning())
	{
		HandleIncomingPackets();
		en::sleep(en::milliseconds(100));
	}
	return true;
}

bool Server::IsRunning() const
{
	return mRunning;
}

void Server::HandleIncomingPackets()
{
	sf::Packet receivedPacket;
	sf::IpAddress remoteAddress;
	en::U16 remotePort;
	while (mSocket.PollPacket(receivedPacket, remoteAddress, remotePort))
	{
		en::U8 packetIDRaw;
		receivedPacket >> packetIDRaw;
		const en::Endpoint endpoint(remoteAddress, remotePort);

		const ClientPacketID packetID = static_cast<ClientPacketID>(packetIDRaw);
		switch (packetID)
		{
		case ClientPacketID::Ping:
		{
			SendPongPacket(endpoint);
		} break;
		case ClientPacketID::Pong:
		{
		} break;
		case ClientPacketID::Join:
		{
			en::U32 playerIndex = GetIndexFromEndpoint(endpoint);
			const bool slotAvailable = (static_cast<en::U32>(mPlayers.size()) < defaultMaxPlayers);
			if (slotAvailable && playerIndex == en::U32_Max)
			{
				LogInfo(en::LogChannel::All, 5, "Player joined from %s:%d", remoteAddress.toString().c_str(), remotePort);

				Player newPlayer;
				newPlayer.endpoint = endpoint;
				mPlayers.push_back(newPlayer);

				playerIndex = static_cast<en::U32>(mPlayers.size() - 1);
				SendConnectionAcceptedPacket(endpoint, playerIndex, GameData::sReactionTime);
				SendClientJoinedPacket(playerIndex);

				LogInfo(en::LogChannel::All, 5, "%d players connected", mPlayers.size());

				if (mPlayers.size() == defaultMaxPlayers)
				{
					GameData::InitGame(mPlayers.size());
					SendGameInfo();
				}
			}
		} break;
		case ClientPacketID::Leave:
		{
			const en::U32 playerIndex = GetIndexFromEndpoint(endpoint);
			if (playerIndex != en::U32_Max)
			{
				LogInfo(en::LogChannel::All, 5, "Player left from %s:%d", remoteAddress.toString().c_str(), remotePort);

				SendClientLeftPacket(playerIndex);
				
				mPlayers.erase(mPlayers.begin() + playerIndex);
			}
			else
			{
				LogInfo(en::LogChannel::All, 5, "Unknown player left from %s:%d", remoteAddress.toString().c_str(), remotePort);
			}
		} break;

		case ClientPacketID::Reaction:
		{
			const en::U32 playerIndex = GetIndexFromEndpoint(endpoint);
			if (playerIndex != en::U32_Max && mPlayers.size() == defaultMaxPlayers)
			{
				if (playerIndex == GameData::sCurrentPlayer)
				{
					en::U32 reactionRaw;
					receivedPacket >> reactionRaw;
					ReactionData::Type reaction = static_cast<ReactionData::Type>(reactionRaw);
					if (GameData::IsReactionAvailable(reaction) > 0)
					{
						LogInfo(en::LogChannel::All, 5, "Player react:%d from %s:%d", reactionRaw, remoteAddress.toString().c_str(), remotePort);
						GameData::React(reaction);
						SendGameInfo();
					}
				}
			}
		} break;

		default:
		{
			LogWarning(en::LogChannel::All, 6, "Unknown ClientPacketID %d received from %s:%d", packetIDRaw, remoteAddress.toString().c_str(), remotePort);
		} break;
		}
	}
}

en::U32 Server::GetIndexFromEndpoint(const en::Endpoint& endpoint) const
{
	const en::U32 size = static_cast<en::U32>(mPlayers.size());
	for (en::U32 i = 0; i < size; ++i)
	{
		if (endpoint == mPlayers[i].endpoint)
		{
			return i;
		}
	}
	return en::U32_Max;
}

void Server::SendToAllPlayers(sf::Packet& packet)
{
	const en::U32 size = static_cast<en::U32>(mPlayers.size());
	for (en::U32 i = 0; i < size; ++i)
	{
		mSocket.SendPacket(packet, mPlayers[i].endpoint.GetAddress(), mPlayers[i].endpoint.GetPort());
	}
}

void Server::SendPingPacket(const en::Endpoint& endpoint)
{
	if (mSocket.IsRunning() && endpoint.GetPort() != 0)
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::Ping);
		mSocket.SendPacket(packet, endpoint.GetAddress(), endpoint.GetPort());
	}
}

void Server::SendPongPacket(const en::Endpoint& endpoint)
{
	if (mSocket.IsRunning() && endpoint.GetPort() != 0)
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::Pong);
		mSocket.SendPacket(packet, endpoint.GetAddress(), endpoint.GetPort());
	}
}

void Server::SendConnectionAcceptedPacket(const en::Endpoint& endpoint, en::U32 playerIndex, const en::Time& timePerReaction)
{
	if (mSocket.IsRunning() && endpoint.GetPort() != 0)
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::ConnectionAccepted);
		packet << playerIndex;
		packet << timePerReaction.asSeconds();
		mSocket.SendPacket(packet, endpoint.GetAddress(), endpoint.GetPort());
	}
}

void Server::SendConnectionRejectedPacket(const en::Endpoint& endpoint)
{
	if (mSocket.IsRunning() && endpoint.GetPort() != 0)
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::ConnectionRejected);
		mSocket.SendPacket(packet, endpoint.GetAddress(), endpoint.GetPort());
	}
}

void Server::SendClientJoinedPacket(en::U32 playerIndex)
{
	if (mSocket.IsRunning())
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::ClientJoined);
		packet << playerIndex;
		SendToAllPlayers(packet);
	}
}

void Server::SendClientLeftPacket(en::U32 playerIndex)
{
	if (mSocket.IsRunning())
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::ClientLeft);
		packet << playerIndex;
		SendToAllPlayers(packet);
	}
}

void Server::SendServerStopPacket()
{
	if (mSocket.IsRunning())
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::Stopping);
		mSocket.SetBlocking(true);
		SendToAllPlayers(packet);
		mSocket.SetBlocking(false);
	}
}

void Server::SendGameInfo()
{
	if (mSocket.IsRunning())
	{
		sf::Packet packet;
		packet << static_cast<en::U8>(ServerPacketID::GameInfo);
		packet << static_cast<en::U8>(GameData::sNumberOfPlayers);
		packet << static_cast<en::U8>(GameData::sCurrentTurn);
		packet << static_cast<en::U8>(GameData::sCurrentPlayer);
		packet << static_cast<en::U8>(GameData::sFailed);
		packet << GameData::sCards[GameData::sCardWhoID].text;
		packet << GameData::sCards[GameData::sCardWhatID].text;
		packet << GameData::sCards[GameData::sCardWhereID].text;
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)];
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)];
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)];
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)];
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)];
		packet << GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)];
		SendToAllPlayers(packet);
	}
}
