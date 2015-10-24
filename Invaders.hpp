#pragma once


#include "Invader.hpp"

#include <tuple>


class Player;


class Invaders final : private sf::NonCopyable
{
	using InvadersNode		= std::vector<SceneNode*>;
	using ConstantIterator	= std::vector<SceneNode*>::const_iterator;


public:
							Invaders();

							template<typename... Targs>
	void					addEnemy(Targs&&...);

	void					push(SceneNode* invaders);
	void					clear();
	bool					empty() const;
	ConstantIterator		begin() const;
	ConstantIterator		end() const;

	void					requstChangeDirectionCommands();
	void					updateCommand(CommandQueue& commands);
	void					update(Player& player, float line, bool& end);


private:
	bool					mIsRequstCommand;
	InvadersNode			mInvaders;
};


template<typename... Targs>
void Invaders::addEnemy(Targs&&... args)
{
	std::tuple<Targs...> tuple(std::forward<Targs>(args)...);

	auto enemy(std::make_unique<Invader>(	std::get<0>(tuple),
											std::get<3>(tuple),
											std::get<4>(tuple),
											std::get<5>(tuple)));

	auto Invader(enemy.get());
	mInvaders.push_back(Invader);
	mInvaders.back()->setPosition(std::get<1>(tuple), std::get<2>(tuple));

	std::get<6>(tuple)->attachChild(std::move(enemy));
}