#include "Component.hpp"


GUI::Component::Component()
	: mIsSelected(false)
	, mIsActive(false)
{
}

bool GUI::Component::isSelected() const
{
	return mIsSelected;
}

void GUI::Component::select()
{
	mIsSelected = true;
}

void GUI::Component::deselect()
{
	mIsSelected = false;
}

bool GUI::Component::isActive() const
{
	return mIsActive;
}

void GUI::Component::activate()
{
	mIsActive = true;
}

void GUI::Component::deactivate()
{
	mIsActive = false;
}

bool GUI::Component::contains(sf::Vector2f position) const
{
	return false;
}