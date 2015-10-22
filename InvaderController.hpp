#pragma once


#include <SFML/System/NonCopyable.hpp>


class CommandQueue;


class InvaderController final : private sf::NonCopyable
{
public:
							InvaderController();

	void					requstChangeDirectionCommands();
	void					update(CommandQueue& commands);


private:
	bool					mIsRequstCommand;
};