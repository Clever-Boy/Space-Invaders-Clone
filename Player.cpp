#include "Player.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace
{
	const std::vector<PlayerData> Table = initializePlayerData();
}


Player::Player(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mExplosion(textures.get(Textures::PlayerExplosion))
	, mFireCommand()
	, mFireRateLevel(Table[type].fireRate)
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsMarkedForRemoval(false)
	, mAnimateCountdown(sf::Time::Zero)
	, mTimer(sf::Time::Zero)
	, mAnimateRate(Table[type].animateRate)
	, mIsHit(false)
{
	mExplosion.setTextureRect(Table[type].textureRectExplosion);
	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);

	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = std::bind(&Player::createBullets, this, std::placeholders::_1, std::cref(textures));
}

void Player::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: mark for removal
	if (isDestroyed())
	{
		mIsMarkedForRemoval = true;
		playLocalSound(commands, SoundEffect::PlayerExplosion);
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// player get hit
	checkForHit(dt);

	Entity::updateCurrent(dt, commands);
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed())
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

bool Player::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Player::playerMover(float vx, float vy)
{
	accelerate(sf::Vector2f(vx, vy) * getMaxSpeed());
}

void Player::onHit()
{
	mIsHit = true;
}

void Player::checkForHit(sf::Time dt)
{
	auto textureRect(mSprite.getTextureRect());

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

void Player::apllyHitEffect(sf::Time dt)
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

unsigned int Player::getCategory() const
{
	return Category::PlayerSpaceship;
}

sf::FloatRect Player::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Player::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Player::fire()
{
	mIsFiring = true;
}

void Player::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		playLocalSound(commands, SoundEffect::PlayerGunfire);

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

void Player::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::PlayerBullet;

	createProjectile(node, type, 0.f, 0.5f, textures);

}

void Player::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	auto  projectile(std::make_unique<Projectile>(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0.f, projectile->getMaxSpeed());

	auto sign = -1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);

	node.attachChild(std::move(projectile));
}

void Player::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}