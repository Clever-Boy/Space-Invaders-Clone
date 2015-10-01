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
	const std::vector<PlayerData>& Table = initializePlayerData();
}


Player::Player(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mFireCommand()
	, mIsFiring(false)
	, mReadyToFire(true)
	, mBullet(nullptr)
	, mIsMarkedForRemoval(false)
	, mAnimateRate(Table[type].animateRate)
	, mAnimateCountdown(sf::Time::Zero)
	, mPlayExplosionSound(true)
{
	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);

	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneSpaceLayer;
	mFireCommand.action = std::bind(&Player::createBullets, this, std::placeholders::_1, std::cref(textures));
}

void Player::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mBullet != nullptr)
	{
		if (mBullet->isDestroyed())
		{
			mReadyToFire = true;
		}
	}

	// Check if bullets are fired
	checkProjectileLaunch(dt, commands);

	Entity::updateCurrent(dt, commands);
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

bool Player::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Player::setMarkToRemove()
{
	mIsMarkedForRemoval = true;
}

void Player::playerMover(float vx, float vy)
{
	accelerate(sf::Vector2f(vx, vy) * getMaxSpeed());
}

void Player::applyHitEffect(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: play Explosion sound 
	if (isDestroyed())
	{
		if (mPlayExplosionSound)
		{
			playLocalSound(commands, SoundEffect::PlayerExplosion);
			mPlayExplosionSound = false;
		}
	}

	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	auto textureRect(mSprite.getTextureRect());

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
	if (!mIsFiring)
		return;

	mIsFiring = false;

	if (!mReadyToFire)
		return;

	commands.push(mFireCommand);
	playLocalSound(commands, SoundEffect::PlayerGunfire);
	mReadyToFire = false;
}

void Player::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	auto type = Projectile::PlayerBullet;

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
	mBullet = projectile.get();
	node.attachChild(std::move(projectile));
}

void Player::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	auto worldPosition(getWorldPosition());

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, effect, worldPosition));

	commands.push(command);
}