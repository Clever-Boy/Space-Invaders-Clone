#include "Life.hpp"
#include "Utility.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PlayerData>& Table = initializePlayerData();
}

Life::Life(Player::Type type, const TextureHolder& textures)
	: mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mDrity(true)
{
	centerOrigin(mSprite);
	mSprite.setColor(Table[type].color);
	setScaleSize(mSprite, 2, 1);
}

void Life::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mDrity)
	{
		mTransform.combine(getTransform());
		mDrity = false;
	}

	states.transform = mTransform;
	target.draw(mSprite, states);
}