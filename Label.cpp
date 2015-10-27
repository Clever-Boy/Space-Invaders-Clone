#include "Label.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


GUI::Label::Label(const std::string& text, const FontHolder& fonts)
	: mText(text, fonts.get(Fonts::Main), 16)
{
}

bool GUI::Label::isSelectable() const
{
	return false;
}

void GUI::Label::handleEvent(const sf::Event&, const sf::Vector2f)
{
}

void GUI::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.combine(getTransform());

	target.draw(mText, states);
}

void GUI::Label::setText(const std::string& text)
{
	mText.setString(text);
}