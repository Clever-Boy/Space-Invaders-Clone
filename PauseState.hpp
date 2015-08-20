#pragma once


#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState final : public State
{
public:
	explicit			PauseState(StateStack& stack, Context context);

	void				draw() override;
	bool				update(sf::Time dt) override;
	bool				handleEvent(const sf::Event& event) override;


private:
	sf::Sprite			mBackgroundSprite;
	sf::Text			mPausedText;
	GUI::Container 		mGUIContainer;
};