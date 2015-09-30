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


Boss::Boss(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Dirction dirction)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mDirectionIndex((dirction == MovingRight) ? +1.f : -1.f )
	, mIsMarkedForRemoval(false)
	, mExplosion()
	, mShowExpolsion(true)
	, mPlayedExplosionSound(false)
	, mPlayedSound(false)
	, mBounds()
{
	mBounds.left -= 100;
	mBounds.width += bounds.width + 100 * 2;

	mBounds.top += bounds.top;
	mBounds.height += bounds.height;

	mExplosion.setTexture(textures.get(Textures::EnemiesExplosion));
	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	mSprite.setColor(Table[type].color);

	centerOrigin(mSprite);
}

void Boss::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExpolsion)
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
		if (!mPlayedExplosionSound && mShowExpolsion)
		{
			playLocalSound(commands, SoundEffect::EnemiesExplosion);

			mPlayedExplosionSound = true;
		}

		return;
	}

	// Play sound 
	playMovementSounds(dt, commands);

	// delete it if boss goes out of range
	if (!mBounds.contains(getWorldPosition()))
		remove();

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
	auto vx = getMaxSpeed() * mDirectionIndex;
	auto vy = 0.f;

	setVelocity(vx, vy);
}

void Boss::playMovementSounds(sf::Time dt, CommandQueue& commands)
{
	if (!mPlayedSound)
	{
		playLocalSound(commands, SoundEffect::BossMovements);

		mPlayedSound = true;
	}

	mTimer += dt;

	if (mTimer > sf::seconds(3.f))
	{
		playLocalSound(commands, SoundEffect::BossMovements);

		mTimer = sf::Time::Zero;
	}
}

void Boss::remove()
{
	mShowExpolsion = false;
	Entity::remove();
}

void Boss::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}