#pragma once


#include <SFML/System/NonCopyable.hpp>


class CommandQueue;


class InvadersController : private sf::NonCopyable
{
public:
							InvadersController();

	void					requstChangeDirectionCommands();
	void					update(CommandQueue& commands);


private:
	bool					mIsRequstCommand;
};