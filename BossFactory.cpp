#include "BossFactory.hpp"
#include "Boss.hpp"
#include "GameConstants.hpp"


BossFactory::BossFactory(const TextureHolder& textures, const sf::FloatRect& bounds)
	: mSceneNode(nullptr)
	, mBoss(nullptr)
	, mTextures(textures)
	, mBounds(bounds)
	, mTimer(sf::Time::Zero)
	, mSpawn(true)
	, mFirstSpawn(true)
{
}

void BossFactory::setSceneNode(SceneNode* node)
{
	mSceneNode = node;
}

Boss* BossFactory::spawnBoss(sf::Time dt) const
{
	Boss::Direction direction;
	auto position = 0.f;

	mTimer += dt;

	if (mFirstSpawn && mTimer > sf::seconds(5.f))
	{
		auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mBounds, Boss::MovingLeft));
		mBoss = boss.get();
		mBoss->setPosition(mBounds.left + mBounds.width + MovementsPadding, Padding * 1.5f);
		mSceneNode->attachChild(std::move(boss));
		mTimer = sf::Time::Zero;
		mFirstSpawn = false;
		return mBoss;
	}

	if (mTimer < sf::seconds(20.f))
		return mBoss;

	if (mSpawn)
	{
		direction = Boss::MovingRight;
		position = -MovementsPadding;
	}
	else
	{
		direction = Boss::MovingLeft;
		position = mBounds.left + mBounds.width + MovementsPadding;
	}

	mSpawn = !mSpawn;

	// Receate Boss
	auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mBounds, direction));
	mBoss = boss.get();
	mBoss->setPosition(position, Padding * 1.5f);
	mSceneNode->attachChild(std::move(boss));
	mTimer = sf::Time::Zero;
	return mBoss;
}