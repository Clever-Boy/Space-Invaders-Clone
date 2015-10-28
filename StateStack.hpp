#pragma once


#include "State.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <map>


namespace sf
{
	class RenderWindow;
}


class StateStack final : private sf::NonCopyable
{
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::None);

		Action				action;
		States::ID			stateID;
	};

	using StatesFactoryMap = std::map<States::ID, std::function<State::Ptr()>>;
	using StatesContainer  = std::vector<State::Ptr>;
	using PendingContainer = std::vector<PendingChange>;


public:
	explicit				StateStack(State::Context context);

							template <typename T>
	void					registerState(States::ID stateID);

	void					update(sf::Time dt);
	void					draw();
	void					handleEvent(const sf::Event& event);

	void					pushState(States::ID stateID);
	void					popState();
	void					clearStates();

	bool					isEmpty() const;


private:
	State::Ptr				createState(States::ID stateID);
	void					applyPendingChanges();


private:
	StatesContainer			mStack;
	PendingContainer		mPendingList;

	State::Context			mContext;
	StatesFactoryMap		mFactories;
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	static_assert(std::is_base_of<State, T>::value, "!");

	mFactories.emplace(std::make_pair(stateID, [this]()
	{
		return std::move(std::make_unique<T>(*this, mContext));
	}));
}