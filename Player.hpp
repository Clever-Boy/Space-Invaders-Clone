#pragma once


#include "Entity.hpp"
#include "Command.hpp"
#include "Projectile.hpp"


class Player final : public Entity
{
public:
	enum Type
	{
		PlayerShip,
		TypeCount
	};


public:
	explicit				Player(Type type, const TextureHolder& textures);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;
	bool 					isMarkedForRemoval() const override;

	void					playerMover(float vx, float vy);
	void 					fire();
	void					onHit();


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

	void					checkForHit(sf::Time dt);
	void					apllyHitEffect(sf::Time dt);
	float					getMaxSpeed() const;

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

	int						mAnimateRate;
	sf::Time				mAnimateCountdown;
	sf::Time				mTimer;
	bool					mIsHit;
};