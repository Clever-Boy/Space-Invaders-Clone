#include "Boss.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<BossData> Table	= initializeBossData();
	constexpr auto ListenerZ			= 300.f;
	constexpr auto Attenuation			= 8.f;
	constexpr auto MinDistance2D		= 200.f;
	const float MinDistance3D			= std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}


Boss::Boss(Type type, const TextureHolder& textures, const sf::FloatRect& bounds, Dirction dirction)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mDirectionIndex((dirction == MovingRight) ? +1.f : -1.f )
	, mIsMarkedForRemoval(false)
	, mExplosion(textures.get(Textures::EnemiesExplosion))
	, mShowExpolsion(true)
	, mBounds()
	, mSound()
	, mSoundBuffers()
{
	mSoundBuffers.load(SoundEffect::BossMovements, "Media/Sounds/BossMovements.wav");

	mSound.setBuffer(mSoundBuffers.get(SoundEffect::BossMovements));
	mSound.setPosition(getWorldPosition().x, -getWorldPosition().y, 0.f);
	mSound.setAttenuation(Attenuation);
	mSound.setMinDistance(MinDistance3D);

	mBounds.left -= 100;
	mBounds.width += bounds.width + 100 * 2;

	mBounds.top += bounds.top;
	mBounds.height += bounds.height;

	mExplosion.setColor(Table[type].color);
	centerOrigin(mExplosion);

	setScaleSize(mSprite, Table[type].size.x, Table[type].size.y);
	mSprite.setColor(Table[type].color);

	centerOrigin(mSprite);
}

void Boss::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExpolsion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Boss::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: mark for removal
	if (isDestroyed())
	{
		if (mSound.getStatus() == sf::Sound::Playing)
			mSound.stop();

		mIsMarkedForRemoval = true;
		return;
	}

	// Play sound 
	if (mSound.getStatus() == sf::Sound::Stopped)
		mSound.play();

	// delete it if boss goes out of range
	if (!mBounds.contains(getWorldPosition()))
		remove();

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);

	Entity::updateCurrent(dt, commands);
}

unsigned int Boss::getCategory() const
{
	return Category::BossSpaceship;
}

sf::FloatRect Boss::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Boss::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

float Boss::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Boss::updateMovementPattern(sf::Time dt)
{
	auto vx = getMaxSpeed() * mDirectionIndex;
	auto vy = 0.f;

	setVelocity(vx, vy);
}

void Boss::remove()
{
	mShowExpolsion = false;
	Entity::remove();
}
