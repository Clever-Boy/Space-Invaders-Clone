#pragma once


#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>


class MenuState final : public State
{
public:
	explicit				MenuState(StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;


private:
	sf::Sprite				mBackgroundSprite;
	GUI::Container			mGUIContainer;
};