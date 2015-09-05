#pragma once


#include "Command.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <map>


class CommandQueue;

class Player final : private sf::NonCopyable
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		Fire,
		ActionCount
	};

	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};


public:
	Player();

	void									handleEvent(const sf::Event& event, CommandQueue& commands);
	void									handleRealtimeInput(CommandQueue& commands);

	void									assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key						getAssignedKey(Action action) const;

	void 									setMissionStatus(MissionStatus status);
	MissionStatus 							getMissionStatus() const;


private:
	void									initializeActions();
	static bool								isRealtimeAction(Action action);


private:
	std::map<sf::Keyboard::Key, Action>		mKeyBinding;
	std::map<Action, Command>				mActionBinding;
	MissionStatus 							mCurrentMissionStatus;
};
