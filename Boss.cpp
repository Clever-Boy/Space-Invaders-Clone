#include "Boss.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<BossData>& Table = data::initializeBossData();
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
{
	using namespace utility;

	const auto SpawnPadding = 60.f;

	mBounds.left	-= SpawnPadding;
	mBounds.width	+= bounds.width + SpawnPadding * 2;

	mBounds.top		+= bounds.top;
	mBounds.height	+= bounds.height;

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
		stopLocalSound(commands);
		mIsMarkedForRemoval = true;
		return;
	}

	// Play sound 
	playLocalSound(commands);

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

void Boss::playLocalSound(CommandQueue& commands)
{
	auto worldPosition(getWorldPosition());

	Command command;
	command.category	= Category::SoundEffect;
	command.action		= derivedAction<SoundNode>(std::bind(&SoundNode::playRepeatedSound, std::placeholders::_1, worldPosition));

	commands.push(command);
}

void Boss::stopLocalSound(CommandQueue& commands)
{
	Command command;
	command.category	= Category::SoundEffect;
	command.action		= derivedAction<SoundNode>(std::bind(&SoundNode::stopRepeatedSound, std::placeholders::_1));

	commands.push(command);
}
