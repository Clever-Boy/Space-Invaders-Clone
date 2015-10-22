#include "BossFactory.hpp"
#include "GameConstants.hpp"


BossFactory::BossFactory(const TextureHolder& textures, const sf::FloatRect& bounds)
	: mSceneNode(nullptr)
	, mBoss(nullptr)
	, mTextures(textures)
	, mBounds(bounds)
	, mTimer(sf::Time::Zero)
	, mPosition(0.f, Padding * 1.5f)
	, mDirection(Boss::MovingLeft)
	, mSpawn(true)
	, mIsAlive(true)
{
}

void BossFactory::setSceneNode(SceneNode* node)
{
	mSceneNode = node;
}

Boss* BossFactory::create(Boss::Direction direction, sf::Vector2f poisition) const
{
	auto boss(std::make_unique<Boss>(Boss::BossShip, mTextures, mBounds, direction));
	mBoss = boss.get();
	mBoss->setPosition(poisition);
	mSceneNode->attachChild(std::move(boss));

	return mBoss;
}

Boss* BossFactory::spawn() const
{
	if (mIsAlive)
		return mBoss;

	mIsAlive = true;

	return create(mDirection, mPosition);
}

void BossFactory::update(sf::Time dt)
{
	if (mBoss != nullptr && (!mBoss->isDestroyed() || !mIsAlive))
		return;

	mTimer += dt;

	if (mTimer < sf::seconds(10.f))
		return;

	if (mSpawn)
	{
		mDirection = Boss::MovingRight;
		mPosition.x = -MovementsPadding;
	}
	else
	{
		mDirection = Boss::MovingLeft;
		mPosition.x = mBounds.left + mBounds.width + MovementsPadding;
	}

	mSpawn = !mSpawn;
	mTimer = sf::Time::Zero;
	mIsAlive = false;

}