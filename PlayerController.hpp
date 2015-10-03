#pragma once


#include "Command.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <map>


class CommandQueue;


class PlayerController final : private sf::NonCopyable
{
public:
	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};

	enum Action
	{
		MoveLeft,
		MoveRight,
		Fire,
		ActionCount
	};


private:
	using KeyMap	= std::map<sf::Keyboard::Key, Action>;
	using ActionMap = std::map<Action, Command>;


public:
							PlayerController();

	void					handleEvent(const sf::Event& event, CommandQueue& commands);
	void					handleRealtimeInput(CommandQueue& commands);

	void					assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key		getAssignedKey(Action action) const;

	void 					setMissionStatus(MissionStatus status);
	MissionStatus 			getMissionStatus() const;


private:
	void					initializeActions();
	static bool				isRealtimeAction(Action action);


private:
	KeyMap					mKeyBinding;
	ActionMap				mActionBinding;
	MissionStatus 			mCurrentMissionStatus;
};
