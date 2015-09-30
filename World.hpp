#pragma once


#include "SpriteNode.hpp"
#include "Boss.hpp"
#include "Invaders.hpp"
#include "Player.hpp"
#include "CommandQueue.hpp"
#include "QuadTree.hpp"
#include "Life.hpp"
#include "Shield.hpp"
#include "SoundPlayer.hpp"

#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics\Text.hpp>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World final : private sf::NonCopyable
{
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

	void					addLifes();
	void					addLife(float relX, float relY);

	void					addShields();
	void					addShield(float relX, float relY);

	void					updateText();

	sf::FloatRect			getBattlefieldBounds() const;
	sf::FloatRect			getMovementsfieldBounds() const;

	void					updateSounds();

	void					enemyProjectileCollision();
	void					playerProjectileCollision();
	void					enemyCollision();

	void					adaptEnemyMovements();


private:
	enum Layer
	{
		Background,
		Space,
		LayerCount
	};

	using LayerContainer	= std::array<SceneNode*, LayerCount>;
	using LivesContainer	= std::list<std::unique_ptr<Life>>;
	using NodeContainer		= std::vector<SceneNode*>;


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

	Player*					mPlayerShip;

	QuadTree				mQuadTreePrimary;
	QuadTree				mQuadTreeSecondary;
	NodeContainer			mEnemyNodes;
	NodeContainer			mPlayerBulletNodes;
	NodeContainer			mEnemyBulletNodes;

	LivesContainer			mLives;

	float					mDeadLine;
	std::size_t				mScore;
	sf::Text				mScoreText;
	sf::Text				mStaticScoreText;
	sf::Text				mLivesText;
	SoundPlayer&			mSounds;

	sf::Time				mTimer;
	bool					mBossSpawn;
	bool					mFirstSpawn;
};
