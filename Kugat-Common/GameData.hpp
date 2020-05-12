#pragma once

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/System/String.hpp>
#include <SFML/Network/Packet.hpp>
#include <string>
#include <vector>

struct CardData
{
	enum class Type : en::U32
	{
		Who = 0,
		What = 1,
		Where = 2,
		Count
	};

	enum class Tag : en::U64
	{
		None = 0,
		Solo = 1,
		Duo = 2,
		Group = 4,
		Benevole = 8,
	};

	en::U32 id;
	Type type;
	en::I32 bonus;
	en::U32 nb;
	std::string text;
	en::U64 tag;
};

struct ReactionData
{
	enum class Type : en::U32
	{
		Firefighter = 0,
		Volunteer = 1,
		Police = 2,
		Security = 3,
		Safety = 4,
		Assistant = 5,
		Nothing = 6,
		Count
	};

	Type type;
	en::I32 bonus;
	en::U32 nb;
};

struct GameData
{
	struct CardDelay
	{
		en::U32 id;
		en::U32 delay;
	};
	struct ReactionDelay
	{
		ReactionData::Type type;
		en::U32 delay;
	};

	// Game stats
	static bool sLoaded;
	static en::U32 sLoadedVersion;
	static en::Time sReactionTime;
	static en::U32 sFailedMax;
	static en::U32 sFailedThreshold;
	static en::U32 sDelayCard;
	static en::U32 sDelayReaction;
	static std::vector<CardData> sCards;
	static std::vector<ReactionData> sReactions;

	static bool LoadFromFile(const std::string& filename);

	// Game values
	static en::U32 sNumberOfPlayers;
	static en::U32 sCurrentTurn;
	static en::U32 sCurrentPlayer;
	static en::U32 sFailed;
	static en::Time sAccumulatedTime;
	static en::U32 sCardWhoID;
	static en::U32 sCardWhereID;
	static en::U32 sCardWhatID;
	static en::U32 sCanPlay[static_cast<en::U32>(ReactionData::Type::Count)]; // TODO
	static std::vector<CardDelay> sRecentCardPile;
	static std::vector<ReactionDelay> sRecentReactionPile;

	static void InitGame(en::U32 playerCount);
	static bool React(ReactionData::Type reaction);
	static en::U32 IsReactionAvailable(ReactionData::Type reaction);

private:
	static en::U32 GetRandomCard(CardData::Type cardType);
};