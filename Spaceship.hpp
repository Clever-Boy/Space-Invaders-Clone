#pragma once


#include "Entity.hpp"
#include "Command.hpp"
#include "Projectile.hpp"


class Spaceship final : public Entity
{
public:
	enum Type
	{
		Player,
		Boss,
		Enemy1,
		Enemy2,
		Enemy3,
		TypeCount
	};


public:
	explicit		Spaceship(Type type, const TextureHolder& textures);

	unsigned int	getCategory() const override;
	sf::FloatRect	getBoundingRect() const override;
	bool 			isMarkedForRemoval() const override;
	bool			isAllied() const;
	float			getMaxSpeed() const;

	void			playerMover(float vx, float vy);
	void 			fire();
	void			remove() override;
	void			setMaxSpeed(float point);
	Type			getType() const;
	void			onHit();

	void			playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

	void			requestChangeDirection(bool ChangeDirction);


private:
	void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 			updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void			updateMovementPattern(sf::Time dt);
	void			applyAnimation(sf::Time dt);
	bool			isAnimated() const;

	void			checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void			createBullets(SceneNode& node, const TextureHolder& textures) const;
	void			createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

	void			checkForHit(sf::Time dt);
	void			apllyHitEffect(sf::Time dt);


private:
	Type			mType;
	sf::Sprite		mSprite;

	Command 		mFireCommand;
	sf::Time		mFireCountdown;
	bool 			mIsFiring;
	bool 			mIsMarkedForRemoval;
	int				mFireRateLevel;

	float			mTravelledDistance;
	std::size_t		mDirectionIndex;

	int				mAnimateRate;
	sf::Time		mAnimateCountdown;
	sf::Time		mTimer;
	bool 			mShowExplosion;

	float			mMaxSpeed;

	sf::Sprite		mExplosion;
	bool			mIsHit;
	bool			mPlayedExplosionSound;

	bool			mChaneDirction;
};
