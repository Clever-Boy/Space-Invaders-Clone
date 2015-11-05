#pragma once


#include "Command.hpp"
#include "Projectile.hpp"
#include "Random.hpp"


class Invaders;


class Invader final : public Entity
{
public:
	enum Type
	{
		Enemy1,
		Enemy2,
		Enemy3,
		TypeCount
	};


private:
	enum Direction
	{
		MovingRight,
		MovingLeft,
		MovingDown
	};


public:
	explicit				Invader(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Invaders& invaders);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;

	float					getMaxSpeed() const;
	void					setMaxSpeed(float point);

	void 					fire();
	Type					getType() const;
	void					requstChangeDirection();


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					updateMovementPattern(sf::Time dt);
	void					applyAnimation(sf::Time dt);

	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

	bool 					isMarkedForRemoval() const override;

	void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);


private:
	Type					mType;
	sf::Sprite				mSprite;
	sf::Sprite				mExplosion;

	Command 				mFireCommand;
	std::size_t				mFireRateLevel;
	sf::Time				mFireCountdown;
	bool 					mIsFiring;

	bool 					mIsMarkedForRemoval;

	float					mTravelledDistance;
	Direction				mCurrentDirection;
	Direction				mPreviousDirection;
	sf::Vector2f			mMovement;

	std::size_t				mAnimateRate;
	sf::Time				mAnimateCountdown;

	float					mMaxSpeed;

	sf::FloatRect			mBounds;
	Invaders&				mInvaders;
	bool					mIsChangeDirection;

	RandomInt				mRandom;
};
