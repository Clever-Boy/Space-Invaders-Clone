#pragma once


#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState final : public State
{
public:
	explicit			TitleState(StateStack& stack, Context context);

	void				draw() override;
	bool				update(sf::Time dt) override;
	bool				handleEvent(const sf::Event& event) override;


private:
	sf::Sprite			mBackgroundSprite;
	sf::Text			mText;

	bool				mShowText;
	sf::Time			mTextEffectTime;
};