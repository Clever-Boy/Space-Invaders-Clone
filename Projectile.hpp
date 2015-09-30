#pragma once


#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Projectile final : public Entity
{
public:
	enum Type
	{
		PlayerBullet,
		EnemyBullet,
		TypeCount
	};


public:
	explicit				Projectile(Type type, const TextureHolder& textures);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;
	float					getMaxSpeed() const;
	int						getDamage() const;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
	Type					mType;
	sf::Sprite				mSprite;
};
