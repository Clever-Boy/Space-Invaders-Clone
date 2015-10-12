#include "InvadersController.hpp"
#include "CommandQueue.hpp"
#include "Invaders.hpp"


InvadersController::InvadersController()
	: mIsRequstCommand(false)
{
}

void InvadersController::requstChangeDirectionCommands()
{
	mIsRequstCommand = true;
}

void InvadersController::update(CommandQueue& commands)
{
	if (!mIsRequstCommand)
		return;

	mIsRequstCommand = false;

	Command command;
	command.category = Category::EnemySpaceship;
	command.action = derivedAction<Invaders>(std::bind(&Invaders::requstChangeDirection, std::placeholders::_1));

	commands.push(command);
}