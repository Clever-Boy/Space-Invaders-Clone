#pragma once


#include "SpriteNode.hpp"
#include "Boss.hpp"
#include "Invaders.hpp"
#include "Player.hpp"
#include "CommandQueue.hpp"
#include "QuadTree.hpp"
#include "LifeNode.hpp"
#include "Shield.hpp"
#include "SoundPlayer.hpp"
#include "InvadersController.hpp"
#include "ScoreNode.hpp"

#include <SFML\Graphics\View.hpp>


// Forward declaration
namespace sf
{
	class RenderWindow;
}


class World final : private sf::NonCopyable
{
	enum Layer
	{
		Background,
		Space,
		LayerCount
	};

	using LayerContainer = std::array<SceneNode*, LayerCount>;
	using NodeContainer	 = std::vector<SceneNode*>;


public:
	explicit				World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);

	void					update(sf::Time dt);
	void					draw();
	bool 					hasAlivePlayer() const;
	bool					hasPlayerWon() const;
	CommandQueue&			getCommandQueue();


private:
	void					loadTextures();
	void					buildScene();
	void					adaptPlayerPosition();

	void					addEnemies();
	void					addEnemy(Invaders::Type type, float relX, float relY);
	void					spawnBoss(sf::Time dt);

	sf::FloatRect			getViewBounds() const;

	void					destroyEntitiesOutsideView();
	void					handleCollisions();
	void					checkForCollision();

	void					controlEnemyFire();

	void					addShields();
	void					addShield(float relX, float relY);

	sf::FloatRect			getBattlefieldBounds() const;
	sf::FloatRect			getMovementsfieldBounds() const;

	void					updateSounds();

	void					enemyProjectileCollision();
	void					playerProjectileCollision();
	void					enemyCollision();

	bool					checkPlayerDeath(sf::Time dt);
	void					spawnPlayer();


private:
	sf::RenderTarget&		mTarget;
	FontHolder&				mFonts;

	sf::View				mWorldView;
	TextureHolder			mTextures;
	ImageHolder				mImages;

	SceneNode				mSceneGraph;
	LayerContainer			mSceneLayers;
	CommandQueue			mCommandQueue;

	sf::FloatRect			mWorldBounds;
	sf::Vector2f			mSpawnPosition;

	Player*					mPlayer;
	Boss*					mBoss;
	LifeNode*				mLife;
	ScoreNode*				mScore;

	QuadTree				mQuadTreePrimary;
	QuadTree				mQuadTreeSecondary;

	NodeContainer			mEnemyNodes;
	NodeContainer			mPlayerBulletNodes;
	NodeContainer			mEnemyBulletNodes;

	float					mDeadLine;

	SoundPlayer&			mSounds;

	sf::Time				mBossTimer;
	bool					mBossSpawn;
	bool					mFirstSpawn;

	sf::Time				mPlayerTimer;
	bool					mIsPlayerDead;
	std::size_t				mLivesCount;
	sf::Vector2f			mPreviousPosition;

	bool					mIsGameEnded;
	bool					mEndGame;

	InvadersController		mInvadersController;
};
