#pragma once


// Entity/scene node category, used to dispatch commands and collisions
namespace Category
{
	enum Type
	{
		None				= 0,
		SceneSpaceLayer		= 1 << 0,
		PlayerSpaceship		= 1 << 1,
		BossSpaceship		= 1 << 2,
		EnemySpaceship		= 1 << 3,
		PlayerProjectile	= 1 << 4,
		EnemyProjectile		= 1 << 5,

		Shield				= 1 << 6,
		SoundEffect			= 1 << 7,

		Spaceship			= PlayerSpaceship | BossSpaceship | EnemySpaceship,
		Projectile			= PlayerProjectile | EnemyProjectile,

		All					= Spaceship | Projectile | Shield,
	};
}