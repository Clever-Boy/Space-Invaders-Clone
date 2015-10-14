#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>


class Boss;
class SceneNode;


class BossFactory final : private sf::NonCopyable
{
public:
	explicit				BossFactory(const TextureHolder& teatures, const sf::FloatRect& bounds);

	void					setSceneNode(SceneNode* node);
	Boss*					spawnBoss(sf::Time dt);


private:
	Boss*					mBoss;
	SceneNode*				mSceneNode;
	const TextureHolder&	mTextures;
	sf::FloatRect			mBounds;

	sf::Time				mTimer;
	bool					mSpawn;
	bool					mFirstSpawn;
};