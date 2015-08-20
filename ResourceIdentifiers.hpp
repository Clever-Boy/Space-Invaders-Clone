#pragma once

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		Player,
		Background,
		Enemies,
		Shields,
		Boss,
		Bullet,
		EnemiesExplosion,
		PlayerExplosion,
		TitleScreen,
		Buttons,
	};
}


namespace Fonts
{
	enum ID
	{
		Main,
		Vector,
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
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, SoundEffect::ID>;
