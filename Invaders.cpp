#include "Invaders.hpp"
#include "CommandQueue.hpp"
#include "Player.hpp"


Invaders::Invaders()
	: mIsRequstCommand(false)
	, mInvaders()

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

void Invaders::update(Player& player, float line, bool& end)
{

	std::sort(mInvaders.begin(), mInvaders.end(),
		[this](const auto& lhs, const auto& rhs)
	{
		return lhs->getPosition().y > rhs->getPosition().y;
	});

	for (auto i = 0u, size = mInvaders.size(); i < size; ++i)
	{
		auto& enemy(static_cast<Invader&>(*mInvaders[i]));

		if (enemy.isDestroyed())
			continue;

		if (enemy.getWorldPosition().y >= line)
		{
			player.destroy();
			end = true;
		}

		if (mInvaders.size() <= 3)
			enemy.setMaxSpeed(enemy.getMaxSpeed() + 1.f);

		if (i < 11)
			enemy.fire();
	}
}