#include "Invaders.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace
{
	const std::vector<InvadersData> Table = initializeInvadersData();
}


Invaders::Invaders(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mExplosion(textures.get(Textures::EnemiesExplosion))
	, mFireCommand()
	, mFireRateLevel(Table[type].fireRate)
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsMarkedForRemoval(false)
	, mTravelledDistance()
	, mCurrentDirction(Dirction::MovingRight)
	, mPreviousDirction(Dirction::MovingRight)
	, mMovement(1, 0)
	, mMaxSpeed(Table[mType].speed)
	, mAnimateCountdown(sf::Time::Zero)
	, mAnimateRate(Table[type].animateRate)
{
	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	mSprite.setColor(Table[type].color);

	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = std::bind(&Invaders::createBullets, this, std::placeholders::_1, std::cref(textures));
}

void Invaders::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed())
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Invaders::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: mark for removal
	if (isDestroyed())
	{
		mIsMarkedForRemoval = true;
		playLocalSound(commands, SoundEffect::EnemiesExplosion);
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);

	applyAnimation(dt);

	Entity::updateCurrent(dt, commands);
}

unsigned int Invaders::getCategory() const
{
	return Category::EnemySpaceship;
}

Invaders::Type Invaders::getType() const
{
	return mType;
}

sf::FloatRect Invaders::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Invaders::getMaxSpeed() const
{
	return mMaxSpeed;
}

void Invaders::setMaxSpeed(float point)
{
	mMaxSpeed = point;
}

void Invaders::fire()
{
	if (randomInt(50) == 0)
		mIsFiring = true;
}

float Invaders::getTravelledDistance() const
{
	return mTravelledDistance;
}

void Invaders::requstChangeDirction()
{
	if (mCurrentDirction == MovingRight)
	{
		mCurrentDirction = MovingDown;
		mPreviousDirction = MovingRight;
		mTravelledDistance = 0;
		mMovement = sf::Vector2f(0, 1);
		return;
	}

	if (mCurrentDirction == MovingLeft)
	{
		mCurrentDirction = MovingDown;
		mPreviousDirction = MovingLeft;
		mTravelledDistance = 0;
		mMovement = sf::Vector2f(0, 1);
		return;
	}

	if (mCurrentDirction == MovingDown)
	{
		if (mPreviousDirction == MovingLeft)
		{
			mCurrentDirction = MovingRight;
			mMovement = sf::Vector2f(1, 0);
		}
		else
		{
			mCurrentDirction = MovingLeft;
			mMovement = sf::Vector2f(-1, 0);
		}

		mPreviousDirction = MovingDown;
		mTravelledDistance = 0;
	}
}

Invaders::Dirction Invaders::getCurrentDirction() const
{
	return mCurrentDirction;
}

void Invaders::updateMovementPattern(sf::Time dt)
{
	setVelocity(mMovement * getMaxSpeed());

	mTravelledDistance += getMaxSpeed() * dt.asSeconds();
}

void Invaders::applyAnimation(sf::Time dt)
{
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	auto textureRect(mSprite.getTextureRect());

	if (mAnimateCountdown <= sf::Time::Zero)
	{
		mAnimateCountdown += Table[mType].animationInterval / (Table[mType].animateRate + 1.f);

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

void Invaders::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		playLocalSound(commands, SoundEffect::EnemiesGunfire);

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

void Invaders::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::EnemyBullet;

	createProjectile(node, type, 0.f, 0.5f, textures);
}

void Invaders::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	auto  projectile(std::make_unique<Projectile>(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0.f, projectile->getMaxSpeed());

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(velocity);

	node.attachChild(std::move(projectile));
}

bool Invaders::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Invaders::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}