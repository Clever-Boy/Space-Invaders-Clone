#include "LifeNode.hpp"
#include "Utility.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<PlayerData>& Table = data::initializePlayerData();
}


LifeNode::LifeNode(Player::Type type, const TextureHolder& textures)
	: mSprites()
{
	using namespace utility;

	const auto LivesCount	= 2u;
	const auto Padding		= 50.f;
	const sf::Vector2f position(700.f, 25.f);

	for (auto i = 0u; i < LivesCount; ++i)
	{
		mSprites.push_back(sf::Sprite(textures.get(Table[type].texture), Table[type].textureRect));
		mSprites.back().setPosition(position.x + Padding * i, position.y);
		setScaleSize(mSprites.back(), 2, 1);
		centerOrigin(mSprites.back());
	}
}

void LifeNode::decrement()
{
	if (!mSprites.empty())
		mSprites.pop_back();
}

void LifeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& i : mSprites)
		target.draw(i, states);
}