#include "InvadersController.hpp"
#include "Invaders.hpp"


InvadersController::InvadersController(CommandQueue& commands)
	: mCommands(commands)
	, mIsRequstCommand(false)
{
}

void InvadersController::requstChangeDirectionCommands()
{
	mIsRequstCommand = true;
}

void InvadersController::update()
{
	if (!mIsRequstCommand)
		return;

	mIsRequstCommand = false;

	Command command;
	command.category = Category::EnemySpaceship;
	command.action = derivedAction<Invaders>(std::bind(&Invaders::requstChangeDirction, std::placeholders::_1));

	mCommands.push(command);
}