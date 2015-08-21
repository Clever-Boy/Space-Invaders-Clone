#include "Shield.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace
{
	const std::vector<ShieldData> Table = initializeShieldData();
}


Shield::Shield(Type type, const TextureHolder& textures)
	: Entity(3)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	centerOrigin(mSprite);
	setScaleSize(mSprite, Table[type].size.x, Table[0].size.y);
}

void Shield::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Shield::getCategory() const
{
	return Category::Shield;
}

void Shield::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	sf::IntRect textureRect = Table[mType].textureRect;
	if(getHitpoints() == 2)
		textureRect.left = textureRect.width;
	else if(getHitpoints() == 1)
		textureRect.left = textureRect.width * 2;

	mSprite.setTextureRect(textureRect);
}

sf::FloatRect Shield::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}