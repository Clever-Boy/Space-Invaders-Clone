#pragma once


#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode final : public SceneNode
{
public:
	explicit				SpriteNode(const sf::Texture& texture);
							SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	sf::Sprite				mSprite;
};
