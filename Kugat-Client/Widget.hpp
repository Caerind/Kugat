#pragma once

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace UI
{

class Widget
{
public:
	Widget();

	const en::Vector2f& GetPosition() const;
	const en::Vector2f& GetSize() const;
	bool IsEnabled() const;
	bool IsVisible() const;
	en::Rectf GetBounds() const;

	void SetPosition(const en::Vector2f& position);
	void SetPosition(en::F32 x, en::F32 y);
	void SetSize(const en::Vector2f& size);
	void SetSize(en::F32 width, en::F32 height);
	void SetEnabled(bool enabled);
	void SetVisible(bool visible);

private:
	en::Vector2f mPosition;
	en::Vector2f mSize;
	bool mEnabled;
	bool mVisible;
};

} // namespace UI
