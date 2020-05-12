#pragma once

#include "Widget.hpp"

#include <Enlivengine/Application/ResourceManager.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <array>
 
namespace UI
{

class Button : public Widget
{
public:
	Button();

	enum class TextureState
	{
		Enabled = 0,
		Disabled,
		Count
	};
	struct TexturePart
	{
		TexturePart();

		en::ResourceID textureID;
		en::Recti textureRect;
	};
	void SetTexturePart(TextureState state, en::ResourceID textureID, const en::Recti& textureRect);
	void SetTexturePart(TextureState state, TexturePart texturePart);
	const TexturePart& GetTexturePart(TextureState state) const;

private:
	std::array<TexturePart, static_cast<std::size_t>(Button::TextureState::Count)> mTextures;
};

} // namespace UI
