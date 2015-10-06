#include "Label.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{
	Label::Label(const std::string& text, const FontHolder& fonts)
		: mText(text, fonts.get(Fonts::Main), 16)
	{
	}

	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::handleEvent(const sf::Event&, const sf::Vector2f)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mText, states);
	}

	void Label::setText(const std::string& text)
	{
		mText.setString(text);
	}
}