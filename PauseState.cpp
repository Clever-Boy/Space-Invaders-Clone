#include "PauseState.hpp"
#include "Button.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, mBackgroundSprite()
	, mPausedText()
	, mGUIContainer(context.sounds)
{
	using namespace utility;

	auto& font(context.fonts.get(Fonts::Main));
	auto windowSize(context.window.getSize());

	const sf::Vector2f Padding(100.f, 75.f);
	const auto Height = 50.f;

	mPausedText.setFont(font);
	mPausedText.setString("Game Paused");
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto returnButton(std::make_shared<GUI::Button>(context));
	returnButton->setPosition(0.5f * windowSize.x - Padding.x, 0.4f * windowSize.y + Padding.y);
	returnButton->setText("Return");
	returnButton->setCallback([this]()
	{
		requestStackPop();
	});

	auto backToMenuButton(std::make_shared<GUI::Button>(context));
	backToMenuButton->setPosition(0.5f * windowSize.x - Padding.x, 0.4f * windowSize.y + Padding.y + Height);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallback([this]()
	{
		requestStateClear();
		requestStackPush(States::Menu);
	});

	mGUIContainer.pack(returnButton);
	mGUIContainer.pack(backToMenuButton);
}

void PauseState::draw()
{
	auto& window = getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	const auto& window = getContext().window;
	auto position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	mGUIContainer.handleEvent(event, position);
	return false;
}