#include "GameData.hpp"

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/Math/Random.hpp>

bool GameData::sLoaded;
en::U32 GameData::sLoadedVersion;
std::vector<CardData> GameData::sCards;
std::vector<ReactionData> GameData::sReactions;
en::Time GameData::sReactionTime;
en::U32 GameData::sFailedMax;
en::U32 GameData::sFailedThreshold;
en::U32 GameData::sDelayCard;
en::U32 GameData::sDelayReaction;

en::U32 GameData::sNumberOfPlayers;
en::U32 GameData::sCurrentTurn;
en::U32 GameData::sCurrentPlayer;
en::U32 GameData::sFailed;
en::Time GameData::sAccumulatedTime;
en::U32 GameData::sCardWhoID;
en::U32 GameData::sCardWhereID;
en::U32 GameData::sCardWhatID;
bool GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Count)];
std::vector<GameData::CardDelay> GameData::sRecentCardPile;
std::vector<GameData::ReactionDelay> GameData::sRecentReactionPile;

bool GameData::LoadFromFile(const std::string& filename)
{
	sCards.clear();
	sReactions.clear();

	en::ParserXml xml;
	if (!xml.loadFromFile(filename))
	{
		LogError(en::LogChannel::Map, 9, "Can't open file at %s", filename.c_str());
		return false;
	}

	if (!xml.readNode("GameStats"))
	{
		LogError(en::LogChannel::Map, 9, "Can't open file at %s", filename.c_str());
		return false;
	}
	xml.getAttribute("version", sLoadedVersion);
	en::F32 reactionTimeSeconds = 10.0f;
	xml.getAttribute("reactionTime", reactionTimeSeconds);
	sReactionTime = en::seconds(reactionTimeSeconds);
	xml.getAttribute("failedMax", sFailedMax);
	xml.getAttribute("failedThreshold", sFailedThreshold);
	xml.getAttribute("delayCard", sDelayCard);
	xml.getAttribute("delayReaction", sDelayReaction);

	static en::U32 idGenerator;
	idGenerator = 0;

	if (xml.readFirstNode())
	{
		do
		{
			std::string nodeName = xml.getNodeName();
			if (nodeName == "Card")
			{
				CardData cardData;
				cardData.id = idGenerator++;

				en::U32 type = 0;
				xml.getAttribute("type", type);
				cardData.type = static_cast<CardData::Type>(type);

				cardData.bonus = 0;
				xml.getAttribute("bonus", cardData.bonus);

				cardData.nb = 0;
				xml.getAttribute("nb", cardData.nb);

				en::U32 tag = 0;
				xml.getAttribute("tag", tag);
				cardData.tag = static_cast<en::U64>(tag);

				cardData.text = "";
				xml.getValue(cardData.text);
				en::trim(cardData.text);

				sCards.push_back(cardData);
			}
			else if (nodeName == "Reaction")
			{
				ReactionData reactionData;

				en::U32 type = 0;
				xml.getAttribute("type", type);
				reactionData.type = static_cast<ReactionData::Type>(type);

				reactionData.bonus = 0;
				xml.getAttribute("bonus", reactionData.bonus);

				reactionData.nb = 1;
				xml.getAttribute("nb", reactionData.nb);

				sReactions.push_back(reactionData);
			}
			else
			{
				LogError(en::LogChannel::Map, 8, "Unknown data type %s", nodeName.c_str());
			}
		} while (xml.nextSibling());
		xml.closeNode();
	}
	sLoaded = true;
	return true;
}

void GameData::InitGame(en::U32 playerCount)
{
	sNumberOfPlayers = playerCount;

	sCurrentTurn = 0;
	sCurrentPlayer = sNumberOfPlayers + 1;
	sFailed = 0;
	sAccumulatedTime = en::Time::Zero;
	sCardWhoID = en::U32_Max;
	sCardWhereID = en::U32_Max;
	sCardWhatID = en::U32_Max;
	sRecentCardPile.clear();
	sRecentReactionPile.clear();
	React(ReactionData::Type::Nothing); // Init
}

bool GameData::React(ReactionData::Type reaction)
{
	// Apply reaction
	if (sCurrentTurn > 0)
	{
		en::I32 bonusTotal = 0;

		const en::U32 cardCount = static_cast<en::U32>(sCards.size());
		for (en::U32 i = 0; i < cardCount; ++i)
		{
			if (sCards[i].id == sCardWhoID
				|| sCards[i].id == sCardWhereID
				|| sCards[i].id == sCardWhatID)
			{
				bonusTotal += sCards[i].bonus;
			}
		}
		const en::U32 reactionCount = static_cast<en::U32>(sReactions.size());
		for (en::U32 i = 0; i < reactionCount; ++i)
		{
			if (sReactions[i].type == reaction)
			{
				bonusTotal += sReactions[i].bonus;
			}
		}

		en::I32 dice = en::Random::get<en::I32>(1, 6);
		dice += bonusTotal;
		if (dice > sFailedThreshold)
		{
			// Failed
			LogInfo(en::LogChannel::Map, 9, "Dice : %d, Bonus : %d ==> Failed", dice, bonusTotal);
			sFailed++;
			if (sFailed >= sFailedMax)
			{
				return false; // This is the end
			}
		}
		else
		{
			// Success
			LogInfo(en::LogChannel::Map, 9, "Dice : %d, Bonus : %d ==> Success", dice, bonusTotal);
		}

		// Delay reuse of reaction
		if (reaction != ReactionData::Type::Nothing)
		{
			ReactionDelay delay;
			delay.type = reaction;
			delay.delay = sDelayReaction * sNumberOfPlayers;
			sRecentReactionPile.push_back(delay);
		}
	}

	// Move old cards in the recent cards pile
	if (sCurrentTurn > 0)
	{
		CardDelay delay;
		delay.delay = sDelayCard * sNumberOfPlayers;
		delay.id = sCardWhoID;
		sRecentCardPile.push_back(delay);
		delay.id = sCardWhereID;
		sRecentCardPile.push_back(delay);
		delay.id = sCardWhatID;
		sRecentCardPile.push_back(delay);
	}

	// Reduce delay for recent cards pile
	en::U32 recentCardCount = static_cast<en::U32>(sRecentCardPile.size());
	for (en::U32 i = 0; i < recentCardCount; )
	{
		sRecentCardPile[i].delay--;
		if (sRecentCardPile[i].delay <= 0)
		{
			sRecentCardPile.erase(sRecentCardPile.begin() + i);
			recentCardCount--;
		}
		else
		{
			i++;
		}
	}

	// Reduce delay for recent reactions pile
	en::U32 recentReactionCount = static_cast<en::U32>(sRecentReactionPile.size());
	for (en::U32 i = 0; i < recentReactionCount; )
	{
		sRecentReactionPile[i].delay--;
		if (sRecentReactionPile[i].delay <= 0)
		{
			sRecentReactionPile.erase(sRecentReactionPile.begin() + i);
			recentReactionCount--;
		}
		else
		{
			i++;
		}
	}

	// Next player
	sAccumulatedTime = en::Time::Zero;
	sCurrentPlayer++;
	if (sCurrentPlayer >= sNumberOfPlayers)
	{
		// Next turn
		sCurrentPlayer = 0;
		sCurrentTurn++;
	}

	// Pick new cards
	sCardWhoID = GetRandomCard(CardData::Type::Who);
	sCardWhereID = GetRandomCard(CardData::Type::Where);
	sCardWhatID = GetRandomCard(CardData::Type::What);

	// Update possible actions
	for (en::U32 i = 0; i < static_cast<en::U32>(ReactionData::Type::Count); ++i)
	{
		sCanPlay[i] = IsReactionAvailable(static_cast<ReactionData::Type>(i));
	}

	return true; // Keep playing
}

bool GameData::IsReactionAvailable(ReactionData::Type reaction)
{
	if (reaction == ReactionData::Type::Nothing)
	{
		return true;
	}
	en::U32 foundCount = 0;
	const en::U32 recentReactionCount = static_cast<en::U32>(sRecentReactionPile.size());
	for (en::U32 i = 0; i < recentReactionCount; ++i)
	{
		if (sRecentReactionPile[i].type == reaction)
		{
			foundCount++;
		}
	}
	return foundCount < sReactions[static_cast<en::U32>(reaction)].nb;
}

en::U32 GameData::GetRandomCard(CardData::Type cardType)
{
	std::vector<en::U32> cardsAvailable;

	const en::U32 cardsCount = static_cast<en::U32>(GameData::sCards.size());
	for (en::U32 i = 0; i < cardsCount; ++i)
	{
		if (GameData::sCards[i].type == cardType)
		{
			en::U32 foundCountInRecentPile = 0;
			const en::U32 recentCardCount = static_cast<en::U32>(sRecentCardPile.size());
			for (en::U32 j = 0; j < recentCardCount; ++j)
			{
				if (sRecentCardPile[j].id == GameData::sCards[i].id)
				{
					foundCountInRecentPile++;
				}
			}
			const en::U32 initialCount = GameData::sCards[i].nb;
			const en::U32 delta = initialCount - foundCountInRecentPile;
			for (en::U32 j = 0; j < delta; ++j)
			{
				cardsAvailable.push_back(GameData::sCards[i].id);
			}
		}
	}

	assert(cardsAvailable.size() > 0);
	const en::U32 randomIndex = en::Random::get<en::U32>(0, static_cast<en::U32>(cardsAvailable.size() - 1));
	assert(randomIndex >= 0 && randomIndex < static_cast<en::U32>(cardsAvailable.size()));

	return cardsAvailable[randomIndex];
}