#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Audio/Music.hpp>


class MusicPlayer final : private sf::NonCopyable
{
public:
	MusicPlayer();

	void						play(Music::ID theme);
	void						stop();

	void						setPaused(bool paused);
	void						setVolume(float volume);


private:
	sf::Music							mMusic;
	std::map<Music::ID, std::string>	mFilenames;
	float								mVolume;
};