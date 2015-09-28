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


public:
	explicit		Invaders(Type type, const TextureHolder& textures);

	unsigned int	getCategory() const override;
	sf::FloatRect	getBoundingRect() const override;

	float			getMaxSpeed() const;

	void 			fire();
	void			remove() override;
	void			setMaxSpeed(float point);
	Type			getType() const;

	void			playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

	void			requestChangeDirection(bool ChangeDirction);


private:
	void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 			updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void			updateMovementPattern(sf::Time dt);
	void			applyAnimation(sf::Time dt);

	void			checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void			createBullets(SceneNode& node, const TextureHolder& textures) const;
	void			createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;


private:
	Type			mType;
	sf::Sprite		mSprite;

	Command 		mFireCommand;
	sf::Time		mFireCountdown;
	bool 			mIsFiring;
	int				mFireRateLevel;

	float			mTravelledDistance;
	std::size_t		mDirectionIndex;
	bool			mChaneDirction;

	int				mAnimateRate;
	sf::Time		mAnimateCountdown;

	float			mMaxSpeed;

	sf::Sprite		mExplosion;
	bool 			mShowExplosion;
};
