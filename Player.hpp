#pragma once


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

	void					accelerate(float vx, float vy);
	void 					fire();
	void					applyHitEffect(sf::Time dt, CommandQueue& commands);
	void					setMarkToRemove();


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

	float					getMaxSpeed() const;

	void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);


private:
	Type					mType;
	sf::Sprite				mSprite;

	Command 				mFireCommand;
	bool 					mIsFiring;
	bool					mIsReadyToFire;
	mutable Projectile*		mBullet;

	bool 					mIsMarkedForRemoval;

	std::size_t				mAnimateRate;
	sf::Time				mAnimateCountdown;

	bool					mPlayExplosionSound;
};