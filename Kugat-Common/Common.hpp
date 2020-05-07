#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

// Server -> Client
enum class ServerPacketID : en::U8
{
	Ping = 0,
	Pong,
	ConnectionAccepted,
	ConnectionRejected,
	ClientJoined,
	ClientLeft,
	Stopping,

	// Game specific packets
	GameInfo,

	// Last packet ID
	Count
};
static_assert(static_cast<en::U32>(ServerPacketID::Count) < 255);

// Client -> Server
enum class ClientPacketID : en::U8
{
	Ping = 0,
	Pong,
	Join,
	Leave,

	// Game specific packet
	Reaction,

	// Last packet ID
	Count
};
static_assert(static_cast<en::U32>(ClientPacketID::Count) < 255);

