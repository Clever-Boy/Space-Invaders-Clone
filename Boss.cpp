#include "Boss.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<BossData> Table = initializeBossData();
}


Boss::Boss(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mTravelledDistance()
	, mDirectionIndex()
	, mIsMarkedForRemoval(false)
	, mExplosion()
	, mPlayedExplosionSound(false)
{
	mExplosion.setTexture(textures.get(Textures::EnemiesExplosion));
	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	mSprite.setColor(Table[type].color);

	centerOrigin(mSprite);
}

void Boss::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.combine(getTransform());

	if (isDestroyed())
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Boss::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: mark for removal
	if (isDestroyed())
	{
		mIsMarkedForRemoval = true;

		// Play explosion sound only once
		if (!mPlayedExplosionSound)
		{
			playLocalSound(commands, SoundEffect::EnemiesExplosion);

			mPlayedExplosionSound = true;
		}

		return;
	}

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);

	Entity::updateCurrent(dt, commands);
}

unsigned int Boss::getCategory() const
{
	return Category::BossSpaceship;
}

sf::FloatRect Boss::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Boss::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

float Boss::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Boss::updateMovementPattern(sf::Time dt)
{
	// Enemy Spaceships: Movement pattern
	const std::vector<Direction>& directions = Table[mType].directions;

	// Moved long enough in horizontal direction: flip direction to move aside
	if ( mTravelledDistance > directions[mDirectionIndex].distance)
	{
		mDirectionIndex = (mDirectionIndex + 1) % directions.size();
		mTravelledDistance = 0.f;
	}

	// Compute velocity from direction
	auto radians = toRadian(directions[mDirectionIndex].angle + 90.f);
	auto vx = getMaxSpeed() * std::cos(radians);
	auto vy = getMaxSpeed() * std::sin(radians);

	setVelocity(vx, vy);

	mTravelledDistance += getMaxSpeed() * dt.asSeconds();
}

void Boss::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}