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

	enum Dirction
	{
		MovingRight,
		MovingLeft
	};


public:
	explicit				Boss(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Dirction dirction);

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;
	bool 					isMarkedForRemoval() const override;


private:
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void 					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					updateMovementPattern(sf::Time dt);
	float					getMaxSpeed() const;
	void					remove() override;

	void					playLocalSound(CommandQueue& commands);
	void					stopLocalSound(CommandQueue& commands);


private:
	Type					mType;
	sf::Sprite				mSprite;
	sf::Sprite				mExplosion;

	float					mDirectionIndex;

	bool 					mIsMarkedForRemoval;

	bool					mShowExpolsion;
	sf::FloatRect			mBounds;
};