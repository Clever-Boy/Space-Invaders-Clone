#pragma once


#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState final : public State
{
public:
	explicit			GameState(StateStack& stack, Context context);

	void				draw() override;
	bool				update(sf::Time dt) override;
	bool				handleEvent(const sf::Event& event) override;


private:
	World				mWorld;
	Player&				mPlayer;
};