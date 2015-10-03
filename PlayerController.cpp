#include "PlayerController.hpp"
#include "CommandQueue.hpp"
#include "Player.hpp"


PlayerController::PlayerController()
	: mCurrentMissionStatus(MissionRunning)
{
	// Set initial key bindings
	mKeyBinding.insert(std::make_pair(sf::Keyboard::Left, MoveLeft));
	mKeyBinding.insert(std::make_pair(sf::Keyboard::Right, MoveRight));
	mKeyBinding.insert(std::make_pair(sf::Keyboard::Space, Fire));

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerSpaceship;
}

void PlayerController::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void PlayerController::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for (const auto& pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void PlayerController::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding.insert(std::make_pair(key, action));
}

sf::Keyboard::Key PlayerController::getAssignedKey(Action action) const
{
	for (const auto& pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void PlayerController::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

PlayerController::MissionStatus PlayerController::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

void PlayerController::initializeActions()
{
	using namespace std::placeholders;

	mActionBinding[MoveLeft].action		= derivedAction<Player>(std::bind(&Player::accelerate, _1, -1.f, 0.f));
	mActionBinding[MoveRight].action	= derivedAction<Player>(std::bind(&Player::accelerate, _1, 1.f, 0.f));
	mActionBinding[Fire].action			= derivedAction<Player>(std::bind(&Player::fire, _1));
}

bool PlayerController::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case Fire:return true;
	default:return false;
	}
}