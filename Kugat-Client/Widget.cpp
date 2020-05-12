#include "Widget.hpp"

namespace UI
{

Widget::Widget()
	: mPosition()
	, mSize()
	, mEnabled(true)
	, mVisible(true)
{
}

const en::Vector2f& Widget::GetPosition() const
{
	return mPosition;
}

const en::Vector2f& Widget::GetSize() const
{
	return mSize;
}

bool Widget::IsEnabled() const
{
	return mEnabled;
}

bool Widget::IsVisible() const
{
	return mVisible;
}

en::Rectf Widget::GetBounds() const
{
	return en::Rectf(mPosition, mSize);
}

void Widget::SetPosition(const en::Vector2f& position)
{
	mPosition = position;
}

void Widget::SetPosition(en::F32 x, en::F32 y)
{
	mPosition.set(x, y);
}

void Widget::SetSize(const en::Vector2f& size)
{
	mSize = size;
}

void Widget::SetSize(en::F32 width, en::F32 height)
{
	mSize.set(width, height);
}

void Widget::SetEnabled(bool enabled)
{
	mEnabled = enabled;
}

void Widget::SetVisible(bool visible)
{
	mVisible = visible;
}

} // namespace UI