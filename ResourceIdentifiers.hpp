#pragma once

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
	class Image;
}

namespace Textures
{
	enum ID
	{
		Player,
		Background,
		Enemies,
		Boss,
		Bullet,
		EnemiesExplosion,
		PlayerExplosion,
		TitleScreen,
		Buttons,
	};
}

namespace Images
{
	enum ID
	{
		Shield
	};
}

namespace Fonts
{
	enum ID
	{
		Main
	};
}

namespace SoundEffect
{
	enum ID
	{
		PlayerGunfire,
		EnemiesGunfire,
		EnemiesExplosion,
		PlayerExplosion,
		BossMovements,
		Button,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		MissionTheme,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using ImageHolder = ResourceHolder<sf::Image, Images::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, SoundEffect::ID>;
