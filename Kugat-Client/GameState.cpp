#include "GameState.hpp"

#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>

#include "GameSingleton.hpp"

GameState::GameState(en::StateManager& manager)
	: en::State(manager)
{
	mBackground.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("background").Get());
	mScreen.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("background_screen").Get());

	mTextCurrentTurn.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextCurrentTurn.setCharacterSize(20);
	mTextCurrentTurn.setPosition(10.0f, 10.0f);

	mTextCurrentPlayer.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextCurrentPlayer.setCharacterSize(20);
	mTextCurrentPlayer.setPosition(10.0f, 40.0f);

	mTextAccumulatedTime.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextAccumulatedTime.setCharacterSize(20);
	mTextAccumulatedTime.setPosition(10.0f, 70.0f);

	mTextFailed.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextFailed.setCharacterSize(20);
	mTextFailed.setPosition(180.0f, 40.0f);

	mTextCards.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextCards.setCharacterSize(20);
	mTextCards.setPosition(10.0f, 150.0f);
	mTextCards.setFillColor(sf::Color::Black);

	mTextFirefighter.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextFirefighter.setCharacterSize(15);
	mTextFirefighter.setPosition(10.0f, 300.0f); 
	mTextFirefighter.setString("Pompier : A");
	mTextVolunteer.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextVolunteer.setCharacterSize(15);
	mTextVolunteer.setPosition(10.0f, 325.0f);
	mTextVolunteer.setString("Bénévole : Z");
	mTextPolice.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextPolice.setCharacterSize(15);
	mTextPolice.setPosition(10.0f, 350.0f);
	mTextPolice.setString("Police : E");
	mTextSecurity.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextSecurity.setCharacterSize(15);
	mTextSecurity.setPosition(10.0f, 375.0f);
	mTextSecurity.setString("Sécurité : R");
	mTextSafety.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextSafety.setCharacterSize(15);
	mTextSafety.setPosition(10.0f, 400.0f);
	mTextSafety.setString("Secouriste : T");
	mTextAssistant.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextAssistant.setCharacterSize(15);
	mTextAssistant.setPosition(10.0f, 425.0f);
	mTextAssistant.setString("Prévention : Y");
	mTextNothing.setFont(en::ResourceManager::GetInstance().Get<en::Font>("MainFont").Get());
	mTextNothing.setCharacterSize(15);
	mTextNothing.setPosition(10.0f, 450.0f);
	mTextNothing.setString("Rien : U");

	GameData::InitGame(4);
	UpdateTexts();
}

bool GameState::handleEvent(const sf::Event& event)
{
	ENLIVE_PROFILE_FUNCTION();

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)])
		{
			React(ReactionData::Type::Firefighter);
		}
		if (event.key.code == sf::Keyboard::Z && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)])
		{
			React(ReactionData::Type::Volunteer);
		}
		if (event.key.code == sf::Keyboard::E && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)])
		{
			React(ReactionData::Type::Police);
		}
		if (event.key.code == sf::Keyboard::R && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)])
		{
			React(ReactionData::Type::Security);
		}
		if (event.key.code == sf::Keyboard::T && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)])
		{
			React(ReactionData::Type::Safety);
		}
		if (event.key.code == sf::Keyboard::Y && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)])
		{
			React(ReactionData::Type::Assistant);
		}
		if (event.key.code == sf::Keyboard::U && GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Nothing)])
		{
			React(ReactionData::Type::Nothing);
		}
	}

	return false;
}

bool GameState::update(en::Time dt)
{
	ENLIVE_PROFILE_FUNCTION();

	GameData::sAccumulatedTime += dt;
	if (GameData::sAccumulatedTime >= GameData::sReactionTime)
	{
		React(ReactionData::Type::Nothing);
	}
	mTextAccumulatedTime.setString(std::to_string(GameData::sReactionTime.asSeconds() - GameData::sAccumulatedTime.asSeconds()));

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();

	target.setView(GameSingleton::mView.getHandle());

	target.draw(mBackground);
	target.draw(mTextCards);
	target.draw(mScreen);
	target.draw(mTextCurrentTurn);
	target.draw(mTextCurrentPlayer);
	target.draw(mTextAccumulatedTime);
	target.draw(mTextFailed);

	target.draw(mTextFirefighter);
	target.draw(mTextVolunteer);
	target.draw(mTextPolice);
	target.draw(mTextSecurity);
	target.draw(mTextSafety);
	target.draw(mTextAssistant);
	target.draw(mTextNothing);
}

void GameState::React(ReactionData::Type reaction)
{
	if (!GameData::React(reaction))
	{
		GameData::InitGame(4);
	}
	UpdateTexts();
}

void GameState::UpdateTexts()
{
	mTextCurrentTurn.setString("Tour " + std::to_string(GameData::sCurrentTurn));
	mTextCurrentPlayer.setString("Joueur " + std::to_string(GameData::sCurrentPlayer + 1));
	mTextFailed.setString("Failed : " + std::to_string(GameData::sFailed));
#ifdef ENLIVE_DEBUG
	mTextCards.setString(GameData::sCards[GameData::sCardWhoID].text + " (" + std::to_string(GameData::sCards[GameData::sCardWhoID].bonus) + ")\n"
		+ GameData::sCards[GameData::sCardWhatID].text + " (" + std::to_string(GameData::sCards[GameData::sCardWhatID].bonus) + ")\n"
		+ GameData::sCards[GameData::sCardWhereID].text + " (" + std::to_string(GameData::sCards[GameData::sCardWhereID].bonus) + ")");
#else
	mTextCards.setString(GameData::sCards[GameData::sCardWhoID].text + "\n"
		+ GameData::sCards[GameData::sCardWhatID].text + "\n"
		+ GameData::sCards[GameData::sCardWhereID].text);
#endif // ENLIVE_DEBUG

	static sf::Color Grey(128, 128, 128);
	mTextFirefighter.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)] ? sf::Color::White : Grey);
	mTextVolunteer.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)] ? sf::Color::White : Grey);
	mTextPolice.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)] ? sf::Color::White : Grey);
	mTextSecurity.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)] ? sf::Color::White : Grey);
	mTextSafety.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)] ? sf::Color::White : Grey);
	mTextAssistant.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)] ? sf::Color::White : Grey);
	mTextNothing.setFillColor(GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Nothing)] ? sf::Color::White : Grey);
}
