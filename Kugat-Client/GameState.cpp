#include "GameState.hpp"

#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Enlivengine/System/DateTime.hpp>

#include "GameSingleton.hpp"

GameState::GameState(en::StateManager& manager)
	: en::State(manager)
{
	mOnline = false;
	mPlaying = false;
	mPlayerIndex = en::U32_Max;
	mBackground.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("background").Get());
	mScreen.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("background_screen").Get());
	mCardWhoString = "";
	mCardWhatString = "";
	mCardWhereString = "";

	mTextCurrentTurn.setFont(en::ResourceManager::GetInstance().Get<en::Font>("DS-DIGI").Get());
	mTextCurrentTurn.setCharacterSize(18);
	mTextCurrentTurn.setPosition(51, 79);
	mTextCurrentTurn.setColor(sf::Color::Black);
	mTextTime.setFont(en::ResourceManager::GetInstance().Get<en::Font>("DS-DIGI").Get());
	mTextTime.setCharacterSize(18);
	mTextTime.setPosition(261, 79);
	mTextTime.setColor(sf::Color::Black);

	mTextCardWho.setFont(en::ResourceManager::GetInstance().Get<en::Font>("DS-DIGIB").Get());
	mTextCardWho.setCharacterSize(18);
	mTextCardWho.setPosition(360.0f * 0.5f, 124.0f);
	mTextCardWho.setFillColor(sf::Color::Black);
	mTextCardWhat.setFont(en::ResourceManager::GetInstance().Get<en::Font>("DS-DIGIB").Get());
	mTextCardWhat.setCharacterSize(18);
	mTextCardWhat.setPosition(360.0f * 0.5f, 146.0f);
	mTextCardWhat.setFillColor(sf::Color::Black);
	mTextCardWhere.setFont(en::ResourceManager::GetInstance().Get<en::Font>("DS-DIGIB").Get());
	mTextCardWhere.setCharacterSize(18);
	mTextCardWhere.setPosition(360.0f * 0.5f, 168.0f);
	mTextCardWhere.setFillColor(sf::Color::Black);

	mClockSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("clock").Get());
	mClockSprite.setPosition({ 38.0f, 53.0f });
	mClockInitialRect = mClockSprite.getTextureRect();

	mAntoineSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("antoine").Get());
	mAntoineSprite.setPosition({ 360.0f * 0.5f, 229.0f });
	mAntoineInitialRect = mAntoineSprite.getTextureRect();

	mFirefighterSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_firefighter").Get());
	mFirefighterSprite.setPosition({ 195.0f, 276.0f });
	mVolunteerSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_volunteer").Get());
	mVolunteerSprite.setPosition({ 71.0f, 460.0f });
	mPoliceSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_police").Get());
	mPoliceSprite.setPosition({ 34.0f, 276.0f });
	mSecuritySprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_security").Get());
	mSecuritySprite.setPosition({ 57.0f, 368.0f });
	mSafetySprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_safety").Get());
	mSafetySprite.setPosition({ 195.0f, 368.0f });
	mAssistantSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_assistant").Get());
	mAssistantSprite.setPosition({ 195.0f, 460.0f });
	mNothingSprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("button_nothing").Get());
	mNothingSprite.setPosition({ 95.0f, 551.0f });
}

bool GameState::handleEvent(const sf::Event& event)
{
	ENLIVE_PROFILE_FUNCTION();

	const en::Vector2f mPos = getApplication().GetWindow().getCursorPositionView(GameSingleton::mView);
	//printf("%f %f\n", mPos.x, mPos.y);

	if (GameData::sCurrentPlayer == mPlayerIndex && mPlaying)
	{
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) || (event.type == sf::Event::MouseButtonPressed && mFirefighterSprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Firefighter);
		}
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (event.type == sf::Event::MouseButtonPressed && mVolunteerSprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Volunteer);
		}
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) || (event.type == sf::Event::MouseButtonPressed && mPoliceSprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Police);
		}
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) || (event.type == sf::Event::MouseButtonPressed && mSecuritySprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Security);
		}
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T) || (event.type == sf::Event::MouseButtonPressed && mSafetySprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Safety);
		}
		if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)] > 0 &&
			((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y) || (event.type == sf::Event::MouseButtonPressed && mAssistantSprite.getGlobalBounds().contains(en::toSF(mPos)))))
		{
			React(ReactionData::Type::Assistant);
		}
		if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) || (event.type == sf::Event::MouseButtonPressed && mNothingSprite.getGlobalBounds().contains(en::toSF(mPos))))
		{
			React(ReactionData::Type::Nothing);
		}
	}

	return false;
}

bool GameState::update(en::Time dt)
{
	ENLIVE_PROFILE_FUNCTION();

	if (mOnline)
	{
		if (mSocket.IsRunning())
		{
			sf::Packet packet;
			while (mSocket.PollPacket(packet))
			{
				en::U8 packetIDRaw;
				packet >> packetIDRaw;

				const ServerPacketID packetID = static_cast<ServerPacketID>(packetIDRaw);
				switch (packetID)
				{
				case ServerPacketID::Ping:
				{
					sf::Packet pongPacket;
					pongPacket << static_cast<en::U8>(ClientPacketID::Pong);
					mSocket.SendPacket(pongPacket);
				} break;
				case ServerPacketID::Pong:
				{
				} break;
				case ServerPacketID::ConnectionAccepted:
				{
					en::F32 timePerReactionSeconds;
					packet >> mPlayerIndex >> timePerReactionSeconds;
					GameData::sReactionTime = en::seconds(timePerReactionSeconds);
					LogInfo(en::LogChannel::All, 5, "ConnectionAccepted : Player %d", mPlayerIndex + 1);
				} break;
				case ServerPacketID::ConnectionRejected:
				{
					LogInfo(en::LogChannel::All, 5, "ConnectionRejected%s", "");
					mSocket.Stop();
				} break;
				case ServerPacketID::ClientJoined:
				{
					en::U32 playerIndex;
					packet >> playerIndex;
					LogInfo(en::LogChannel::All, 5, "Player %d joined", playerIndex);
				} break;
				case ServerPacketID::ClientLeft:
				{
					en::U32 playerIndex;
					packet >> playerIndex;
					LogInfo(en::LogChannel::All, 5, "Player %d left", playerIndex);
				} break;
				case ServerPacketID::Stopping:
				{
					LogInfo(en::LogChannel::All, 5, "ServerStopping%s", "");
					mSocket.Stop();
				} break;
				case ServerPacketID::GameInfo:
				{
					en::U8 tmp;
					packet >> tmp; GameData::sNumberOfPlayers = static_cast<en::U32>(tmp);
					packet >> tmp; GameData::sCurrentTurn = static_cast<en::U32>(tmp);
					packet >> tmp; GameData::sCurrentPlayer = static_cast<en::U32>(tmp);
					packet >> tmp; GameData::sFailed = static_cast<en::U32>(tmp);
					packet >> mCardWhoString;
					packet >> mCardWhatString;
					packet >> mCardWhereString;
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)];
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)];
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)];
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)];
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)];
					packet >> GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)];
					GameData::sAccumulatedTime = en::Time::Zero;
					GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Nothing)] = 1;
					UpdateTexts();
				} break;

				default:
				{
					LogWarning(en::LogChannel::All, 6, "Unknown ServerPacketID %d received", packetIDRaw);
				} break;
				}
			}

			GameData::sAccumulatedTime += dt;
			if (GameData::sAccumulatedTime >= GameData::sReactionTime && GameData::sCurrentPlayer == mPlayerIndex)
			{
				React(ReactionData::Type::Nothing);
			}
		}
		else
		{
			if (!mSocket.Start(sf::IpAddress::LocalHost, 3458))
			{
				clearStates();
			}
			sf::Packet packet;
			packet << static_cast<en::U8>(ClientPacketID::Join);
			mSocket.SendPacket(packet);
			GameData::InitGame(4);
			UpdateTexts();
		}
	}
	else
	{
		if (!mPlaying)
		{
			GameData::InitGame(4);
			mPlayerIndex = 0;
			mPlaying = true;
			UpdateTexts();
			GameData::sAccumulatedTime = en::Time::Zero;
		}
		else
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				GameData::sAccumulatedTime += dt;
				if (GameData::sAccumulatedTime >= GameData::sReactionTime && GameData::sCurrentPlayer == mPlayerIndex)
				{
					React(ReactionData::Type::Nothing);
				}
			}
		}
	}

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();

	target.setView(GameSingleton::mView.getHandle());
	
	static bool whiteBgInit = false;
	static sf::RectangleShape whiteBg;
	if (!whiteBgInit)
	{
		whiteBg.setSize({ 360,640 });
		whiteBg.setFillColor(sf::Color::White);
	}

	target.draw(whiteBg);

	if (mPlaying)
	{
		// Clock
		en::F32 percent;
		if (GameData::sReactionTime > en::Time::Zero)
		{
			percent = GameData::sAccumulatedTime.asSeconds() / GameData::sReactionTime.asSeconds();
		}
		else
		{
			percent = GameData::sAccumulatedTime.asSeconds() / 10.0f;
		}
		if (percent > 1.0f)
		{
			percent = 1.0f;
		}
		percent = 1.0f - percent;
		sf::IntRect clockRect = mClockInitialRect;
		clockRect.width = (int)((en::F32)mClockInitialRect.width * percent);
		mClockSprite.setTextureRect(clockRect);
		target.draw(mClockSprite);
	}
	target.draw(mTextCardWho);
	target.draw(mTextCardWhat);
	target.draw(mTextCardWhere);
	target.draw(mAntoineSprite);
	target.draw(mTextCurrentTurn);
	target.draw(mTextTime);

	target.draw(mScreen);
	target.draw(mBackground);

	if (mPlaying)
	{
		if (GameData::sCurrentPlayer == mPlayerIndex)
		{
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Firefighter)] > 0)
				target.draw(mFirefighterSprite);
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Volunteer)] > 0)
				target.draw(mVolunteerSprite);
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Police)] > 0)
				target.draw(mPoliceSprite);
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Security)] > 0)
				target.draw(mSecuritySprite);
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Safety)] > 0)
				target.draw(mSafetySprite);
			if (GameData::sCanPlay[static_cast<en::U32>(ReactionData::Type::Assistant)] > 0)
				target.draw(mAssistantSprite);
			target.draw(mNothingSprite);
		}
	}
}

void GameState::UpdateTexts()
{
	mTextCurrentTurn.setString("Tour " + std::to_string(GameData::sCurrentTurn));

	en::DateTime date(2020, 05, 10, 22, 0, 0);
	date += en::minutes(30 * (GameData::sCurrentTurn - 1));
	std::string timeString = std::to_string(date.getHour()) + "h";
	if (date.getMinute() > 0)
	{
		timeString += std::to_string(date.getMinute());
	}
	mTextTime.setString(timeString);

	en::F32 percent = 1.0f - (GameData::sFailedMax - GameData::sFailed) * 0.2f; 
	sf::IntRect antoineRect = mAntoineInitialRect;
	antoineRect.width = (int)((en::F32)mAntoineInitialRect.width * percent);
	mAntoineSprite.setTextureRect(antoineRect);
	mAntoineSprite.setOrigin(mAntoineSprite.getGlobalBounds().width * 0.5f, 0.0f);

	mCardWhoString = GameData::sCards[GameData::sCardWhoID].text;
	mCardWhatString = GameData::sCards[GameData::sCardWhatID].text;
	mCardWhereString = GameData::sCards[GameData::sCardWhereID].text;
	mTextCardWho.setString(mCardWhoString);
	mTextCardWho.setOrigin(mTextCardWho.getGlobalBounds().width * 0.5f, 0.0f);
	mTextCardWhat.setString(mCardWhatString);
	mTextCardWhat.setOrigin(mTextCardWhat.getGlobalBounds().width * 0.5f, 0.0f);
	mTextCardWhere.setString(mCardWhereString);
	mTextCardWhere.setOrigin(mTextCardWhere.getGlobalBounds().width * 0.5f, 0.0f);
}

void GameState::React(ReactionData::Type reaction)
{
	if (mOnline)
	{
		if (mSocket.IsRunning() && mPlayerIndex == GameData::sCurrentPlayer)
		{
			sf::Packet packet;
			packet << static_cast<en::U8>(ClientPacketID::Reaction);
			packet << static_cast<en::U32>(reaction);
			mSocket.SendPacket(packet);
		}
	}
	else
	{
		GameData::React(reaction);
		mPlayerIndex = GameData::sCurrentPlayer;
		UpdateTexts();
	}
}
