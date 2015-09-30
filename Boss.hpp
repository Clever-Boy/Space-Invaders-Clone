#pragma once


#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Boss final : public Entity
{
public:
	enum Type
	{
		BossShip,
		TypeCount
	};


public:
	explicit				Boss(Type type, const TextureHolder& textures);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;

	bool 					isMarkedForRemoval() const override;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					updateMovementPattern(sf::Time dt);
	float					getMaxSpeed() const;

	void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);


private:
	Type					mType;
	sf::Sprite				mSprite;

	float					mTravelledDistance;
	std::size_t				mDirectionIndex;

	bool 					mIsMarkedForRemoval;

	sf::Sprite				mExplosion;
	bool					mPlayedExplosionSound;
};