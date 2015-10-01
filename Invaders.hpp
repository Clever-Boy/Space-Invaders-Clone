#pragma once


#include "Entity.hpp"
#include "Command.hpp"
#include "Projectile.hpp"


class Invaders final : public Entity
{
public:
	enum Type
	{
		Enemy1,
		Enemy2,
		Enemy3,
		TypeCount
	};

	enum Dirction
	{
		MovingRight,
		MovingLeft,
		MovingDown
	};


public:
	explicit				Invaders(Type type, const TextureHolder& textures);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;

	float					getMaxSpeed() const;
	void					setMaxSpeed(float point);

	void 					fire();
	Type					getType() const;

	float					getTravelledDistance() const;
	Dirction				getCurrentDirction() const;
	void					requstChangeDirction();


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
	sf::Time				mFireCountdown;
	bool 					mIsFiring;
	int						mFireRateLevel;
	bool 					mIsMarkedForRemoval;
	float					mTravelledDistance;
	Dirction				mCurrentDirction;
	Dirction				mPreviousDirction;
	sf::Vector2f			mMovement;
	int						mAnimateRate;
	sf::Time				mAnimateCountdown;
	float					mMaxSpeed;
};
