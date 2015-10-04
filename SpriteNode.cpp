#include "SpriteNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


SpriteNode::SpriteNode(Type type, const sf::Texture& texture)
	: mSprite(texture)
	, mLine()
{
	mSprite.setScale(3.f, 4.f);
}

SpriteNode::SpriteNode(Type type, const sf::FloatRect& bounds)
	: mSprite()
	, mLine()
{
	mLine[0] = sf::Vertex(sf::Vector2f(bounds.left, bounds.top + bounds.height));
	mLine[1] = sf::Vertex(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));


	mLine[0].color = mLine[1].color = sf::Color::Green;
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
	target.draw(&mLine[0], 2, sf::Lines);
}