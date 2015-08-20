#pragma once


// Entity/scene node category, used to dispatch commands and collisions
namespace Category
{
	enum Type
	{
		None				= 0,
		SceneSpaceLayer		= 1 << 0,
		PlayerSpaceship		= 1 << 1,
		EnemySpaceship		= 1 << 2,
		PlayerProjectile	= 1 << 3,
		EnemyProjectile		= 1 << 4,

		Shield				= 1 << 5,
		SoundEffect			= 1 << 6,

		Spaceship			= PlayerSpaceship | EnemySpaceship,
		Projectile			= PlayerProjectile | EnemyProjectile,

		All					= Spaceship | Projectile | Shield,
	};
}