#include "InvaderController.hpp"
#include "CommandQueue.hpp"
#include "Invader.hpp"


InvaderController::InvaderController()
	: mIsRequstCommand(false)
{
}

void InvaderController::requstChangeDirectionCommands()
{
	mIsRequstCommand = true;
}

void InvaderController::update(CommandQueue& commands)
{
	if (!mIsRequstCommand)
		return;

	mIsRequstCommand = false;

	Command command;
	command.category = Category::EnemySpaceship;
	command.action = derivedAction<Invader>(std::bind(&Invader::requstChangeDirection, std::placeholders::_1));

	commands.push(command);
}