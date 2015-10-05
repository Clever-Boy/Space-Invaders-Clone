#pragma once


#include "SceneNode.hpp"


class Entity : public SceneNode
{
public:
	explicit				Entity(int hitpoints);

	void					setVelocity(float vx, float vy);
	void					setVelocity(sf::Vector2f velocity);

	int						getHitpoints() const;
	void					damage(int points);
	void					destroy();
	virtual void			remove();

	bool					isDestroyed() const override;


protected:
	void					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	void					accelerate(sf::Vector2f velocity);


private:
	sf::Vector2f			mVelocity;
	int						mHitpoints;
};