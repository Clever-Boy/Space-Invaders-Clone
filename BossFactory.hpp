#pragma once


#include "Boss.hpp"


class BossFactory final : private sf::NonCopyable
{
public:
	explicit				BossFactory(const TextureHolder& textures, const sf::FloatRect& bounds);

	void					setSceneNode(SceneNode* node);
	Boss*					create(Boss::Direction direction, sf::Vector2f poisition) const;
	Boss*					spawn() const;
	void					update(sf::Time dt);


private:
	mutable Boss*			mBoss;
	SceneNode*				mSceneNode;
	const TextureHolder&	mTextures;
	sf::FloatRect			mBounds;

	sf::Time				mTimer;
	sf::Vector2f			mPosition;
	Boss::Direction			mDirection;
	bool					mSpawn;
	mutable bool			mIsAlive;
};