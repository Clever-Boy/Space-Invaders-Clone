#pragma once


#include "CommandQueue.hpp"


class InvadersController
{
public:
	InvadersController(CommandQueue& commands);
	void requstChangeDirectionCommands();
	void update();


private:
	CommandQueue&	mCommands;
	bool			mIsRequstCommand;
};