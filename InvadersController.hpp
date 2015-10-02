#pragma once


#include <SFML/System/NonCopyable.hpp>


class CommandQueue;


class InvadersController : private sf::NonCopyable
{
public:
	explicit				InvadersController(CommandQueue& commands);

	void					requstChangeDirectionCommands();
	void					update();


private:
	CommandQueue&			mCommands;
	bool					mIsRequstCommand;
};