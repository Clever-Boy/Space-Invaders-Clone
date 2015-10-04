#pragma once


#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <array>


class SpriteNode final : public SceneNode
{
	static const auto Vertices	= 2u;
	using LineContainer			= std::array<sf::Vertex, Vertices>;


public:
	explicit				SpriteNode(const sf::Texture& texture);
							SpriteNode(const sf::FloatRect& bounds);


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	sf::Sprite				mSprite;
	LineContainer			mLine;
};
