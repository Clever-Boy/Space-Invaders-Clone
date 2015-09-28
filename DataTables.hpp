#pragma once


#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>


struct Direction
{
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct BossData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::Color						color;
	sf::Vector2f					size;
	std::vector<Direction>			directions;
	sf::IntRect						textureRectExplosion;
};

struct InvadersData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Color						color;
	sf::Vector2f					size;
	sf::Time						fireInterval;
	std::size_t						fireRate;
	std::vector<Direction>			directions;
	sf::Time						animationInterval;
	int								animateRate;
	sf::IntRect						textureRectExplosion;
};

struct PlayerData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Color						color;
	sf::Vector2f					size;
	sf::Time						fireInterval;
	std::size_t						fireRate;
	sf::Time						animationInterval;
	int								animateRate;
	sf::IntRect						textureRectExplosion;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	sf::Color						color;
	sf::Vector2f					size;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

std::vector<BossData>		initializeBossData();
std::vector<InvadersData>	initializeInvadersData();
std::vector<PlayerData>		initializePlayerData();
std::vector<ProjectileData>	initializeProjectileData();
