#pragma once


#include "PlayerController.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
public:
	Application();
	void					run();


private:
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	void					registerStates();


private:
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;
	PlayerController		mPlayerController;

	MusicPlayer				mMusic;
	SoundPlayer				mSounds;

	StateStack				mStateStack;
};