#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>


struct BossData
{
	float				speed;
	Textures::ID		texture;
	sf::Color			color;
	sf::Vector2f		size;
};

struct InvaderData
{
	float				speed;
	Textures::ID		texture;
	sf::IntRect			textureRect;
	sf::Color			color;
	sf::Vector2f		size;
	sf::Time			fireInterval;
	std::size_t			fireRate;
	sf::Time			animationInterval;
	std::size_t			animateRate;
};

struct PlayerData
{
	float				speed;
	Textures::ID		texture;
	sf::IntRect			textureRect;
	sf::Vector2f		size;
	sf::Time			animationInterval;
	std::size_t			animateRate;
};

struct ProjectileData
{
	float				speed;
	sf::Vector2f		size;
	Textures::ID		texture;
};

namespace data
{
	std::vector<BossData>		initializeBossData();
	std::vector<InvaderData>	initializeInvaderData();
	std::vector<PlayerData>		initializePlayerData();
	std::vector<ProjectileData>	initializeProjectileData();
}
