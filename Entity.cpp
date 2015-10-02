#include "Entity.hpp"

#include <cassert>


Entity::Entity(int hitpoints)
	: mVelocity()
	, mHitpoints(hitpoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

void Entity::damage(int points)
{
	assert(points > 0);

	mHitpoints -= points;
}

void Entity::destroy()
{
	mHitpoints = 0;
}

void Entity::remove()
{
	destroy();
}

bool Entity::isDestroyed() const
{
	return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	move(mVelocity * dt.asSeconds());
}