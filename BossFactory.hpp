#pragma once


#include "Boss.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>


class BossFactory final : private sf::NonCopyable
{
public:
	explicit				BossFactory(const TextureHolder& textures, const sf::FloatRect& bounds);

	void					setSceneNode(SceneNode* node);
	Boss*					create(Boss::Direction direction, float xPosition) const;
	Boss*					spawn(sf::Time dt) const;


private:
	mutable Boss*			mBoss;
	SceneNode*				mSceneNode;
	const TextureHolder&	mTextures;
	sf::FloatRect			mBounds;

	mutable sf::Time		mTimer;
	mutable bool			mSpawn;
};