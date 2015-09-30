#include "SoundPlayer.hpp"

#include <SFML/Audio/Listener.hpp>


namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	constexpr auto ListenerZ		= 300.f;
	constexpr auto Attenuation		= 8.f;
	constexpr auto MinDistance2D	= 200.f;
	const float MinDistance3D		= std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

SoundPlayer::SoundPlayer()
	: mSoundBuffers()
	, mSounds()
{
	mSoundBuffers.load(SoundEffect::PlayerGunfire, "Media/Sounds/PlayerGunfire.wav");
	mSoundBuffers.load(SoundEffect::EnemiesGunfire, "Media/Sounds/EnemiesGunfire.wav");
	mSoundBuffers.load(SoundEffect::PlayerExplosion, "Media/Sounds/PlayerExplosion.wav");
	mSoundBuffers.load(SoundEffect::EnemiesExplosion, "Media/Sounds/EnemiesExplosion.wav");
	mSoundBuffers.load(SoundEffect::BossMovements, "Media/Sounds/BossMovements.wav");
	mSoundBuffers.load(SoundEffect::Button, "Media/Sounds/Button.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
	play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
	mSounds.push_back(sf::Sound());
	auto& sound = mSounds.back();

	sound.setBuffer(mSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);

	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if(
		[](const auto& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	auto position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}