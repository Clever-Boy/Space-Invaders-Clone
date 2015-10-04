#pragma once


#include "SceneNode.hpp"
#include "Player.hpp"

#include <list>


class LifeNode final : public SceneNode
{
public:
	explicit 				LifeNode(Player::Type type, const TextureHolder& textures);

	void					decrement();


private:
	void 					draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	std::list<sf::Sprite>	mSprites;
};
