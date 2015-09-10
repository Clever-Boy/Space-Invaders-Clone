#pragma once


#include "Entity.hpp"

#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>


class Shield final : public Entity
{
public:
	enum Type
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Middle,
		TypeCount
	};


public:
	explicit				Shield(Type type, const TextureHolder& textures);


	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	Type					mType;
	sf::Sprite				mSprite;
	mutable bool			mDrity;
	mutable sf::Transform	mTransform;
};

