#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>


class Player;
class CommandQueue;
class SceneNode;


class PlayerFactory final : private sf::NonCopyable
{
public:
	explicit				PlayerFactory(const TextureHolder& textures);

	void					setSceneNode(SceneNode* node);
	Player*					createPlayer(const sf::Vector2f position);
	Player*					spawnPlayer();
	bool					update(sf::Time dt, CommandQueue& commands);


private:
	Player*					mPlayer;
	SceneNode*				mSceneNode;
	const TextureHolder&	mTextures;

	sf::Time				mTimer;
	bool					mIsAlive;
	sf::Vector2f			mPreviousPosition;
};