#pragma once


#include "ResourceIdentifiers.hpp"
#include "Spaceship.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Life final : public sf::Drawable, public sf::Transformable
{
public:
	explicit 				Life(Spaceship::Type type, const TextureHolder& textures);


private:
	void 					draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	sf::Sprite 				mSprite;
	mutable bool			mDrity;
	mutable sf::Transform	mTransform;
};
