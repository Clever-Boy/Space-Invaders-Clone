#include "DataTables.hpp"
#include "Boss.hpp"
#include "Invader.hpp"
#include "Player.hpp"
#include "Projectile.hpp"


std::vector<BossData> data::initializeBossData()
{
	std::vector<BossData> data(Boss::TypeCount);

	data[Boss::BossShip].color = sf::Color(255, 0, 0);
	data[Boss::BossShip].size = sf::Vector2f(3.f, 1.f);
	data[Boss::BossShip].speed = 80.f;
	data[Boss::BossShip].texture = Textures::Boss;

	return data;
}

std::vector<InvaderData> data::initializeInvaderData()
{
	std::vector<InvaderData> data(Invader::TypeCount);

	data[Invader::Enemy1].color = sf::Color(255, 255, 255);
	data[Invader::Enemy1].fireRate = 1u;
	data[Invader::Enemy1].size = sf::Vector2f(1.5f, 1.5f);
	data[Invader::Enemy1].speed = 50.f;
	data[Invader::Enemy1].texture = Textures::Enemies;
	data[Invader::Enemy1].textureRect = sf::IntRect(0, 0, 25, 20);
	data[Invader::Enemy1].fireInterval = sf::seconds(6.f);
	data[Invader::Enemy1].animationInterval = sf::seconds(2.f);
	data[Invader::Enemy1].animateRate = 1u;

	data[Invader::Enemy2].color = sf::Color(255, 255, 255);
	data[Invader::Enemy2].size = sf::Vector2f(1.5f, 1.5f);
	data[Invader::Enemy2].fireRate = 1u;
	data[Invader::Enemy2].speed = 50.f;
	data[Invader::Enemy2].texture = Textures::Enemies;
	data[Invader::Enemy2].textureRect = sf::IntRect(0, 20, 25, 20);
	data[Invader::Enemy2].fireInterval = sf::seconds(6.f);
	data[Invader::Enemy2].animationInterval = sf::seconds(2.f);
	data[Invader::Enemy2].animateRate = 1u;

	data[Invader::Enemy3].color = sf::Color(255, 255, 255);
	data[Invader::Enemy3].size = sf::Vector2f(1.5f, 1.5f);
	data[Invader::Enemy3].fireRate = 1u;
	data[Invader::Enemy3].speed = 50.f;
	data[Invader::Enemy3].texture = Textures::Enemies;
	data[Invader::Enemy3].textureRect = sf::IntRect(0, 40, 25, 20);
	data[Invader::Enemy3].fireInterval = sf::seconds(6.f);
	data[Invader::Enemy3].animationInterval = sf::seconds(2.f);
	data[Invader::Enemy3].animateRate = 1u;

	return data;
}

std::vector<PlayerData> data::initializePlayerData()
{
	std::vector<PlayerData> data(Player::TypeCount);

	data[Player::PlayerShip].size = sf::Vector2f(2.f, 2.f);
	data[Player::PlayerShip].speed = 200.f;

	data[Player::PlayerShip].texture = Textures::Player;
	data[Player::PlayerShip].textureRect = sf::IntRect(0, 0, 30, 20);

	data[Player::PlayerShip].animationInterval = sf::seconds(0.25f);
	data[Player::PlayerShip].animateRate = 4u;

	return data;
}

std::vector<ProjectileData> data::initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::PlayerBullet].speed = 600.f;
	data[Projectile::PlayerBullet].size = sf::Vector2f(0.4f, 0.5f);
	data[Projectile::PlayerBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].speed = 150.f;
	data[Projectile::EnemyBullet].size = sf::Vector2f(0.3f, 0.6f);
	data[Projectile::EnemyBullet].texture = Textures::Bullet;

	return data;
}
