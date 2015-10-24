#pragma once


#include <SFML/System/NonCopyable.hpp>

#include <vector>


class SceneNode;
class CommandQueue;
class Player;


class Invaders final : private sf::NonCopyable
{
	using InvadersNode		= std::vector<SceneNode*>;
	using ConstantIterator	= std::vector<SceneNode*>::const_iterator;


public:
							Invaders();

	void					push(SceneNode* invaders);
	void					clear();
	bool					empty() const;
	ConstantIterator		begin() const;
	ConstantIterator		end() const;

	void					requstChangeDirectionCommands();
	void					updateCommand(CommandQueue& commands);
	void					update(Player& player, float line, bool& end, bool& changeSpeed);


private:
	InvadersNode			mInvaders;
	bool					mIsRequstCommand;
};