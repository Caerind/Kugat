#pragma once

#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Application/AudioSystem.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "GameData.hpp"

class GameState : public en::State
{
public:
	GameState(en::StateManager& manager);

	bool handleEvent(const sf::Event& event);

	bool update(en::Time dt);

	void render(sf::RenderTarget& target);

private:
	void React(ReactionData::Type reaction);
	void UpdateTexts();

private:
	sf::Sprite mBackground;
	sf::Sprite mScreen;

	sf::Text mTextCurrentTurn;
	sf::Text mTextCurrentPlayer;
	sf::Text mTextAccumulatedTime;
	sf::Text mTextFailed;
	sf::Text mTextCards;

	sf::Text mTextFirefighter;
	sf::Text mTextVolunteer;
	sf::Text mTextPolice;
	sf::Text mTextSecurity;
	sf::Text mTextSafety;
	sf::Text mTextAssistant;
	sf::Text mTextNothing;
};