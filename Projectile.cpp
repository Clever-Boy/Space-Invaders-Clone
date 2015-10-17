#include "Projectile.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<ProjectileData>& Table = data::initializeProjectileData();
}


Projectile::Projectile(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(Table[type].texture))
{
	using namespace utility;

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);

	if (type == PlayerBullet)
		mSprite.rotate(180.f);

	centerOrigin(mSprite);
}

unsigned int Projectile::getCategory() const
{
	if (mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::PlayerProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}