#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Graphics/View.hpp>

#include "GameState.hpp"
#include "GameData.hpp"
#include "GameSingleton.hpp"

int main(int argc, char** argv)
{
	if (argc >= 1)
	{
		en::PathManager::GetInstance().SetExecutablePath(argv[0]);
	}
	en::PathManager::GetInstance().SetScreenshotPath("Screenshots/");
	en::Application::GetInstance().Initialize();

	GameData::LoadFromFile(en::PathManager::GetInstance().GetAssetsPath() + "gamestats.xml");

	GameSingleton::mApplication = &en::Application::GetInstance();
	GameSingleton::mApplication->GetWindow().create(sf::VideoMode(1024, 768), "Kugat", sf::Style::Titlebar | sf::Style::Close);
	GameSingleton::mApplication->GetWindow().setSize({ 360.0f, 640.0f });

	GameSingleton::mView.setSize(GameSingleton::mApplication->GetWindow().getSize());
	GameSingleton::mView.setCenter(0.5f * GameSingleton::mView.getSize());
	GameSingleton::mView.setViewport(en::Rectf(0.0f, 0.0f, GameSingleton::mView.getSize().x / GameSingleton::mApplication->GetWindow().getSize().x, GameSingleton::mView.getSize().y / GameSingleton::mApplication->GetWindow().getSize().y));

	GameSingleton::mApplication->Start<GameState>();

	return 0;
}