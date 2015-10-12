#pragma once


#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <array>


class SpriteNode final : public SceneNode
{
public:
	enum Type
	{
		Background,
		Line
	};


private:
	static const auto Vertices	= 2u;
	using LineContainer			= std::array<sf::Vertex, Vertices>;


public:
	explicit				SpriteNode(Type type, const TextureHolder& textures);
							SpriteNode(Type type, const sf::FloatRect& bounds);


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	Type					mType;
	sf::Sprite				mSprite;
	LineContainer			mLine;
};
