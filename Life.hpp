#pragma once


#include "ResourceIdentifiers.hpp"
#include "Player.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Life final : public sf::Drawable, public sf::Transformable, sf::NonCopyable
{
public:
	explicit 				Life(Player::Type type, const TextureHolder& textures);


private:
	void 					draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	sf::Sprite 				mSprite;
	mutable bool			mDrity;
	mutable sf::Transform	mTransform;
};
