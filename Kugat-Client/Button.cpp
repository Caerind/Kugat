#include "Button.hpp"

namespace UI
{

Button::Button()
	: Widget()
	, mTextures()
{
}

Button::TexturePart::TexturePart()
	: textureID(en::InvalidResourceID)
	, textureRect()
{
}

void Button::SetTexturePart(TextureState state, en::ResourceID textureID, const en::Recti& textureRect)
{
	assert(state < TextureState::Count);
	mTextures[static_cast<std::size_t>(state)].textureID = textureID;
	mTextures[static_cast<std::size_t>(state)].textureRect = textureRect;
}

void Button::SetTexturePart(TextureState state, TexturePart texturePart)
{
	assert(state < TextureState::Count);
	mTextures[static_cast<std::size_t>(state)] = texturePart;
}

const Button::TexturePart& Button::GetTexturePart(TextureState state) const
{
	assert(state < TextureState::Count);
	return mTextures[static_cast<std::size_t>(state)];
}

} // namespace UI