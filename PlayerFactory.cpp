#include "PlayerFactory.hpp"
#include "Player.hpp"


PlayerFactory::PlayerFactory(const TextureHolder& textures)
	: mSceneNode(nullptr)
	, mPlayer(nullptr)
	, mTextures(textures)
	, mTimer(sf::Time::Zero)
	, mPosition()
	, mIsAlive(true)
{
}

void PlayerFactory::setSceneNode(SceneNode* node)
{
	mSceneNode = node;
}

Player*	PlayerFactory::createPlayer(sf::Vector2f position) const
{
	auto leader(std::make_unique<Player>(Player::PlayerShip, mTextures));
	mPlayer = leader.get();
	mPlayer->setPosition(position);
	mSceneNode->attachChild(std::move(leader));
	return mPlayer;
}

Player* PlayerFactory::spawnPlayer() const
{
	if (mIsAlive)
		return mPlayer;

	auto leader(std::make_unique<Player>(Player::PlayerShip, mTextures));
	mPlayer = leader.get();
	mPlayer->setPosition(mPosition);

	mSceneNode->attachChild(std::move(leader));
	mIsAlive = true;
	return mPlayer;
}

bool PlayerFactory::update(sf::Time dt, CommandQueue& commands) const
{
	if (!mPlayer->isDestroyed() || !mIsAlive)
		return false;

	mPlayer->applyHitEffect(dt, commands);
	mTimer += dt;

	if (mTimer > sf::seconds(0.25f))
	{
		mPlayer->setMarkToRemove();
		mPosition = mPlayer->getWorldPosition();
		mIsAlive = false;
		mTimer = sf::Time::Zero;
	}

	return true;
}