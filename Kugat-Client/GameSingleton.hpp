#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>

class GameSingleton
{
public:
	static en::Application* mApplication;
	static en::View mView;
};
