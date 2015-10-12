#include "SpriteNode.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


SpriteNode::SpriteNode(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(Textures::Background))
	, mLine()
{
	mSprite.setScale(3.f, 4.f);
}

SpriteNode::SpriteNode(Type type, const sf::FloatRect& bounds)
	: mType(type)
	, mSprite()
	, mLine()
{
	mLine[0] = sf::Vertex(sf::Vector2f(bounds.left, bounds.top + bounds.height));
	mLine[1] = sf::Vertex(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));


	mLine[0].color = mLine[1].color = sf::Color::Green;
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mType == Background)
		target.draw(mSprite, states);
	else
		target.draw(mLine.data(), Vertices, sf::Lines);
}