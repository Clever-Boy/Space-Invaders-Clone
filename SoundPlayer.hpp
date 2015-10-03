#pragma once


#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer final : private sf::NonCopyable
{
public:
							SoundPlayer();

	void					play(SoundEffect::ID effect);
	void					play(SoundEffect::ID effect, sf::Vector2f position);

	void					playRepeatedSound(sf::Vector2f position);
	void					stopRepeatedSound();

	void					removeStoppedSounds();
	void					setListenerPosition(sf::Vector2f position);
	sf::Vector2f			getListenerPosition() const;


private:
	SoundBufferHolder		mSoundBuffers;
	std::list<sf::Sound>	mSounds;
	sf::Sound				mSound;
};