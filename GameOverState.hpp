#pragma once


#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Text.hpp>


class GameOverState final : public State
{
public:
	explicit				GameOverState(StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;


private:
	sf::Text				mGameOverText;
	sf::Time				mElapsedTime;
};