#include "GameState.hpp"
#include "MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(context.window, context.textures, context.fonts, context.sounds)
	, mPlayerController(context.playerController)
{
	mPlayerController.setMissionStatus(PlayerController::MissionRunning);

	// Play game theme
	context.music.stop();
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer())
	{
		mPlayerController.setMissionStatus(PlayerController::MissionFailure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerWon())
	{
		mPlayerController.setMissionStatus(PlayerController::MissionSuccess);
		requestStackPush(States::GameOver);
	}

	auto& commands(mWorld.getCommandQueue());
	mPlayerController.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	auto& commands(mWorld.getCommandQueue());
	mPlayerController.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}