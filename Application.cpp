#include "Application.hpp"
#include "TitleState.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "GameOverState.hpp"

#define FIXED_TIME_STEP_ENABLE

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(800, 600), "Space Invaders", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayerController()
	, mMusic()
	, mSounds()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayerController, mMusic, mSounds))
{
	mWindow.setKeyRepeatEnabled(false);

#ifdef FIXED_TIME_STEP_ENABLE
	mWindow.setVerticalSyncEnabled(false);
#else
	mWindow.setVerticalSyncEnabled(true);
#endif

	mFonts.load(Fonts::Main, "Media/Sansation.ttf");

	mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	mTextures.load(Textures::Buttons, "Media/Textures/Buttons.png");


	registerStates();
	mStateStack.pushState(States::Title);

	mMusic.setVolume(20.f);

}

void Application::run()
{
#ifdef FIXED_TIME_STEP_ENABLE
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		render();
	}
#else
	sf::Clock clock;
	sf::Time deltaTime = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time elapsed = clock.getElapsedTime();
		clock.restart();

		deltaTime += elapsed;

		if (deltaTime > sf::Time::Zero)
		{
			processInput();
			update(deltaTime);
			render();

			if (mStateStack.isEmpty())
				mWindow.close();

			deltaTime = sf::Time::Zero;
		}
	}
#endif
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
}