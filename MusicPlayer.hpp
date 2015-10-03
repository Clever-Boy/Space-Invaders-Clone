#pragma once


#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Audio/Music.hpp>


class MusicPlayer final : private sf::NonCopyable
{
	using ResourceMap = std::map<Music::ID, std::string>;


public:
							MusicPlayer();

	void					play(Music::ID theme);
	void					stop();

	void					setPaused(bool paused);
	void					setVolume(float volume);


private:
	sf::Music				mMusic;
	ResourceMap				mFilenames;
	float					mVolume;
};