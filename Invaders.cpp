#include "Invaders.hpp"
#include "CommandQueue.hpp"
#include "Invader.hpp"
#include "Player.hpp"


Invaders::Invaders()
	: mInvaders()
	, mIsRequstCommand(false)
{
}

void Invaders::push(SceneNode* invaders)
{
	mInvaders.push_back(invaders);
}

void Invaders::clear()
{
	mInvaders.clear();
}

bool Invaders::empty() const 
{ 
	return mInvaders.empty(); 
}

Invaders::ConstantIterator Invaders::begin() const
{
	return mInvaders.begin(); 
}

Invaders::ConstantIterator Invaders::end() const
{ 
	return mInvaders.end(); 
}

void Invaders::requstChangeDirectionCommands()
{
	mIsRequstCommand = true;
}

void Invaders::updateCommand(CommandQueue& commands)
{
	if (!mIsRequstCommand)
		return;

	mIsRequstCommand = false;

	Command command;
	command.category = Category::EnemySpaceship;
	command.action = derivedAction<Invader>(std::bind(&Invader::requstChangeDirection, std::placeholders::_1));

	commands.push(command);
}

void Invaders::update(Player& player, float line, bool& end, bool& changeSpeed)
{
	std::sort(mInvaders.begin(), mInvaders.end(),
		[](const auto& lhs, const auto& rhs)
	{
		return lhs->getPosition().y > rhs->getPosition().y;
	});

	const auto SpeedIncreaseMultiplier = 0.5f;


	auto total			= mInvaders.size();
	auto distroyed		= std::bind(&SceneNode::isDestroyed, std::placeholders::_1);
	auto numberOfKilled = std::count_if(mInvaders.begin(), mInvaders.end(), distroyed);
	auto numberOfAlive	= std::max(1u, total - numberOfKilled);

	for (auto i = 0u; i < total; ++i)
	{
		auto& enemy(static_cast<Invader&>(*mInvaders[i]));

		if (enemy.isDestroyed())
			continue;

		if (enemy.getWorldPosition().y >= line)
		{
			player.destroy();
			end = true;
		}

		if (changeSpeed)
		{
			auto ratio = SpeedIncreaseMultiplier / numberOfAlive + 1;
			auto speed = enemy.getMaxSpeed() * ratio;
			enemy.setMaxSpeed(speed);
		}

		if (i < 11)	
			enemy.fire();
	}

	if(changeSpeed)	
		changeSpeed = false;
}