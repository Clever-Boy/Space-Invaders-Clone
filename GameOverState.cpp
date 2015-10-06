#include "GameOverState.hpp"
#include "Utility.hpp"
#include "PlayerController.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, mGameOverText()
	, mElapsedTime(sf::Time::Zero)
{
	using namespace utility;

	auto& font(context.fonts.get(Fonts::Main));
	auto windowSize(context.window.getSize());

	mGameOverText.setFont(font);

	if (context.playerController.getMissionStatus() == PlayerController::MissionFailure)
		mGameOverText.setString("Mission failed!");
	else
		mGameOverText.setString("Mission successful!");

	mGameOverText.setCharacterSize(70u);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	auto& window(getContext().window);
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape(window.getView().getSize());
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;

	if (mElapsedTime > sf::seconds(3))
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}

	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}