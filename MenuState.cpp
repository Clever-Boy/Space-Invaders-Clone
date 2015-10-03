#include "MenuState.hpp"
#include "Button.hpp"
#include "Utility.hpp"
#include "MusicPlayer.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer(context.sounds)
{
	auto& texture(context.textures.get(Textures::TitleScreen));
	mBackgroundSprite.setTexture(texture);
	mBackgroundSprite.setScale(1.25f, 1.f);

	const sf::Vector2f PositionTopLeft(300, 350);
	const auto Padding = 50.f;

	auto playButton(std::make_shared<GUI::Button>(context));
	playButton->setPosition(PositionTopLeft);
	playButton->setText("Play");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto settingsButton(std::make_shared<GUI::Button>(context));
	settingsButton->setPosition(PositionTopLeft.x, PositionTopLeft.y + Padding);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
	{
		requestStackPush(States::Settings);
	});

	auto exitButton(std::make_shared<GUI::Button>(context));
	exitButton->setPosition(PositionTopLeft.x, PositionTopLeft.y + Padding * 2);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);

	// Play menu theme
	context.music.play(Music::MenuTheme);
}

void MenuState::draw()
{
	auto& window = getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	const auto& window = getContext().window;
	auto position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	mGUIContainer.handleEvent(event, position);
	return false;
}