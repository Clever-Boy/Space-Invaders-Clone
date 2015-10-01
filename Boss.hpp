#pragma once


#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>


class Boss final : public Entity
{
public:
	enum Type
	{
		BossShip,
		TypeCount
	};

	enum Dirction
	{
		MovingRight,
		MovingLeft
	};

public:
	explicit				Boss(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Dirction dirction);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					updateMovementPattern(sf::Time dt);
	float					getMaxSpeed() const;
	bool 					isMarkedForRemoval() const override;
	void					remove() override;


private:
	Type					mType;
	sf::Sprite				mSprite;

	float					mDirectionIndex;

	bool 					mIsMarkedForRemoval;

	sf::Sprite				mExplosion;
	bool					mShowExpolsion;

	SoundBufferHolder		mSoundBuffers;
	sf::FloatRect			mBounds;
	sf::Sound				mSound;
};