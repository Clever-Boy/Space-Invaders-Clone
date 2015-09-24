#include "Spaceship.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace
{
	const std::vector<SpaceshipData> Table = initializeSpaceshipData();
}


Spaceship::Spaceship(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mExplosion()
	, mShowExplosion(true)
	, mFireCommand()
	, mFireRateLevel(Table[type].fireRate)
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsMarkedForRemoval(false)
	, mTravelledDistance()
	, mDirectionIndex()
	, mAnimateCountdown(sf::Time::Zero)
	, mTimer(sf::Time::Zero)
	, mAnimateRate(Table[type].animateRate)
	, mMaxSpeed(Table[mType].speed)
	, mIsHit(false)
	, mPlayedExplosionSound(false)
{
	if (isAllied())
	{
		mExplosion.setTexture(textures.get(Textures::PlayerExplosion));
		mExplosion.setTextureRect(Table[type].textureRectExplosion);
		mExplosion.setColor(Table[type].color);
		centerOrigin(mExplosion);

		setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	}
	else
	{
		mExplosion.setTexture(textures.get(Textures::EnemiesExplosion));
		mExplosion.setColor(Table[type].color);
		centerOrigin(mExplosion);

		setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
		mSprite.setColor(Table[type].color);
	}

	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = std::bind(&Spaceship::createBullets, this, std::placeholders::_1, std::cref(textures));
}

void Spaceship::onHit()
{
	mIsHit = true;
}

void Spaceship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.combine(getTransform());

	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Spaceship::playerMover(float vx, float vy)
{
	accelerate(sf::Vector2f(vx, vy) * getMaxSpeed());
}

void Spaceship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: mark for removal
	if (isDestroyed())
	{
		mIsMarkedForRemoval = true;

		// Play explosion sound only once
		if (!mPlayedExplosionSound)
		{
			SoundEffect::ID soundEffect = isAllied() ? SoundEffect::PlayerExplosion : SoundEffect::EnemiesExplosion;
			playLocalSound(commands, soundEffect);

			mPlayedExplosionSound = true;
		}
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);

	// player get hit
	checkForHit(dt);

	Entity::updateCurrent(dt, commands);
}

void Spaceship::checkForHit(sf::Time dt)
{
	if (!isAllied())
		return;

	sf::IntRect textureRect = mSprite.getTextureRect();

	if (mIsHit)
	{
		apllyHitEffect(dt);
	}

	// start timer
	mTimer += dt;
	if (mTimer > Table[mType].animationInterval)
	{
		mIsHit = false;
		textureRect = sf::IntRect(0, textureRect.top, textureRect.width, textureRect.height);
		mSprite.setTextureRect(textureRect);
		// reset timer
		mTimer = sf::Time::Zero;
		mAnimateCountdown = sf::Time::Zero;
	}
}

void Spaceship::apllyHitEffect(sf::Time dt)
{
	
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = mSprite.getTextureRect();

	if (mAnimateCountdown <= sf::Time::Zero)
	{
		mAnimateCountdown += Table[mType].animationInterval / (Table[mType].animateRate + 1.f);

		if (textureRect.left + textureRect.width < textureBounds.x)
			textureRect.left += textureRect.width;
		else
			textureRect = sf::IntRect(textureRect.width, textureRect.top, textureRect.width, textureRect.height);
	}
	else
	{
		mAnimateCountdown -= dt;
	}

	mSprite.setTextureRect(textureRect);
}

unsigned int Spaceship::getCategory() const
{
	if (isAllied())
		return Category::PlayerSpaceship;
	else
		return Category::EnemySpaceship;
}

Spaceship::Type Spaceship::getType() const
{
	return mType;
}

sf::FloatRect Spaceship::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Spaceship::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

bool Spaceship::isAllied() const
{
	return mType == Player;
}

float Spaceship::getMaxSpeed() const
{
	return mMaxSpeed;
}
void Spaceship::setMaxSpeed(float point)
{
	mMaxSpeed = point;
}

void Spaceship::fire()
{
	if (Table[mType].fireInterval == sf::Time::Zero)
		return;

	if (isAllied())
	{
		mIsFiring = true;
		return;
	}

	// Only ships with fire interval != 0 are able to fire
	if (randomInt(50) == 0)
		mIsFiring = true;

}

void Spaceship::updateMovementPattern(sf::Time dt)
{
	// Enemy Spaceships: Movement pattern
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Moved long enough in current direction: Change direction
		if (mTravelledDistance > directions[mDirectionIndex].distance)
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

		applyAnimation(dt);
	}
}

void Spaceship::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		playLocalSound(commands, isAllied() ? SoundEffect::PlayerGunfire : SoundEffect::EnemiesGunfire);

		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Spaceship::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::PlayerBullet : Projectile::EnemyBullet;

	createProjectile(node, type, 0.0f, 0.5f, textures);

}

void Spaceship::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	auto  projectile(std::make_unique<Projectile>(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	auto sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

bool Spaceship::isAnimated() const
{
	return Table[mType].hasAnimation;
}

void Spaceship::applyAnimation(sf::Time dt)
{
	if (!isAnimated())
		return;


	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = mSprite.getTextureRect();


	if (mAnimateCountdown <= sf::Time::Zero)
	{
		mAnimateCountdown += Table[mType].animationInterval / (Table[mType].animateRate + 1.f );

		if (textureRect.left + textureRect.width < textureBounds.x)
			textureRect.left += textureRect.width;
		else
			textureRect = sf::IntRect(0, textureRect.top, textureRect.width, textureRect.height);
	}
	else
	{
		mAnimateCountdown -= dt;
	}

	mSprite.setTextureRect(textureRect);
}

void Spaceship::remove()
{
	Entity::remove();
	mShowExplosion = false;
}

void Spaceship::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));
	commands.push(command);
}