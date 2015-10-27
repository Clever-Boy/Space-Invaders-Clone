#include "Invader.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"
#include "Invaders.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace
{
	const std::vector<InvaderData>& Table = data::initializeInvaderData();
}


Invader::Invader(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Invaders& invaders)
	: mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mExplosion(textures.get(Textures::EnemiesExplosion))
	, mFireCommand()
	, mFireRateLevel(Table[type].fireRate)
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsMarkedForRemoval(false)
	, mTravelledDistance()
	, mCurrentDirection(Direction::MovingRight)
	, mPreviousDirection(Direction::MovingRight)
	, mMovement(Table[type].movement[MovingRight])
	, mMaxSpeed(Table[type].speed)
	, mAnimateCountdown(sf::Time::Zero)
	, mAnimateRate(Table[type].animateRate)
	, mBounds(bounds)
	, mInvaders(invaders)
	, mIsChangeDirection(false)
{
	using namespace utility;

	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	mSprite.setColor(Table[type].color);
	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = std::bind(&Invader::createBullets, this, std::placeholders::_1, std::cref(textures));
}

void Invader::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed())
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Invader::updateCurrent(sf::Time dt, CommandQueue& commands)
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

unsigned int Invader::getCategory() const
{
	return Category::EnemySpaceship;
}

Invader::Type Invader::getType() const
{
	return mType;
}

sf::FloatRect Invader::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Invader::getMaxSpeed() const
{
	return mMaxSpeed;
}

void Invader::setMaxSpeed(float point)
{
	mMaxSpeed = point;
}

void Invader::fire()
{
	using namespace utility;

	if (randomInt(100) == 0)
		mIsFiring = true;
}

void Invader::requstChangeDirection()
{
	const std::vector<sf::Vector2f>& movement = Table[mType].movement;

	switch (mCurrentDirection)
	{
	case MovingRight:
		mCurrentDirection = MovingDown;
		mPreviousDirection = MovingRight;
		mTravelledDistance = 0.f;
		mMovement = movement[MovingDown];
		break;
	case MovingLeft:
		mCurrentDirection = MovingDown;
		mPreviousDirection = MovingLeft;
		mTravelledDistance = 0.f;
		mMovement = movement[MovingDown];
		break;
	case MovingDown:
		if (mPreviousDirection == MovingLeft)
		{
			mCurrentDirection = MovingRight;
			mMovement = movement[MovingRight];
		}
		else
		{
			mCurrentDirection = MovingLeft;
			mMovement = movement[MovingLeft];
		}

		mPreviousDirection = MovingDown;
		mTravelledDistance = 0.f;
	}
}

void Invader::updateMovementPattern(sf::Time dt)
{
	bool changeDirection = false;
	const auto TravelledDistance = 30.f;

	// Check if we need change dirtection
	if (mCurrentDirection == Invader::MovingDown)
	{
		if (mTravelledDistance > TravelledDistance)
			changeDirection = true;
	}

	if (mCurrentDirection == Invader::MovingRight || mCurrentDirection == Invader::MovingLeft)
	{
		if (!mBounds.contains(getWorldPosition()))
			changeDirection = true;
	}

	// Validate condition of changing direction
	if (!changeDirection)
		mIsChangeDirection = false;

	// Check if we can requst change direction
	if (changeDirection && !mIsChangeDirection)
	{
		mInvaders.requstChangeDirectionCommands();
		mIsChangeDirection = true;
	}

	// Update velocity
	setVelocity(mMovement * getMaxSpeed());

	// Update travel distance
	mTravelledDistance += getMaxSpeed() * dt.asSeconds();
}

void Invader::applyAnimation(sf::Time dt)
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

void Invader::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
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

void Invader::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	auto type = Projectile::EnemyBullet;

	createProjectile(node, type, 0.f, 0.5f, textures);
}

void Invader::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	auto projectile(std::make_unique<Projectile>(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0.f, projectile->getMaxSpeed());

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(velocity);

	node.attachChild(std::move(projectile));
}

bool Invader::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Invader::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	auto worldPosition(getWorldPosition());

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}