#include "BossFactory.hpp"
#include "GameConstants.hpp"


BossFactory::BossFactory(const TextureHolder& textures, const sf::FloatRect& bounds)
	: mSceneNode(nullptr)
	, mBoss(nullptr)
	, mTextures(textures)
	, mBounds(bounds)
	, mTimer(sf::Time::Zero)
	, mSpawn(true)
{
}

void BossFactory::setSceneNode(SceneNode* node)
{
	mSceneNode = node;
}

Boss* BossFactory::createBoss(Boss::Direction direction, float xPosition) const
{
	auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mBounds, direction));
	mBoss = boss.get();
	mBoss->setPosition(xPosition, Padding * 1.5f);
	mSceneNode->attachChild(std::move(boss));
	mTimer = sf::Time::Zero;
	return mBoss;
}

Boss* BossFactory::spawnBoss(sf::Time dt) const
{
	Boss::Direction direction;
	auto position = 0.f;

	mTimer += dt;

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

	return createBoss(direction, position);
}