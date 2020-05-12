#pragma once

#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Application/AudioSystem.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "GameData.hpp"
#include "ClientSocket.hpp"

class GameState : public en::State
{
public:
	GameState(en::StateManager& manager);

	bool handleEvent(const sf::Event& event);

	bool update(en::Time dt);

	void render(sf::RenderTarget& target);

private:
	void UpdateTexts();

	void React(ReactionData::Type reaction);

private:
	bool mOnline;
	bool mPlaying;
	ClientSocket mSocket;
	en::U32 mPlayerIndex;
	sf::Sprite mBackground;
	sf::Sprite mScreen;
	sf::Sprite mClockSprite;
	sf::IntRect mClockInitialRect;
	std::string mCardWhoString;
	std::string mCardWhatString;
	std::string mCardWhereString;
	sf::Text mTextCurrentTurn;
	sf::Text mTextTime;
	sf::Text mTextCardWho;
	sf::Text mTextCardWhat;
	sf::Text mTextCardWhere;
	sf::Sprite mAntoineSprite;
	sf::IntRect mAntoineInitialRect;

	sf::Sprite mFirefighterSprite;
	sf::Sprite mVolunteerSprite;
	sf::Sprite mPoliceSprite;
	sf::Sprite mSecuritySprite;
	sf::Sprite mSafetySprite;
	sf::Sprite mAssistantSprite;
	sf::Sprite mNothingSprite;
};