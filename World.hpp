#pragma once


#include "Invader.hpp"
#include "CommandQueue.hpp"
#include "QuadTree.hpp"
#include "InvaderController.hpp"
#include "ResourceHolder.hpp"
#include "PlayerFactory.hpp"
#include "BossFactory.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>


class LifeNode;
class ScoreNode;
class SoundPlayer;


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
	explicit				World(sf::RenderTarget&	target, FontHolder& fonts, SoundPlayer& sounds);

	void					draw();
	void					update(sf::Time dt);
	bool 					hasAlivePlayer() const;
	bool					hasPlayerWon() const;
	CommandQueue&			getCommandQueue();


private:
	void					loadTextures();
	void					buildScene();

	void					addShields();
	void					addShield(float relX, float relY);

	void					addEnemies();
	void					addEnemy(Invader::Type type, float relX, float relY);
	void					controlEnemyFire();

	void					adaptPlayerPosition();

	sf::FloatRect			getViewBounds() const;
	sf::FloatRect			getBattlefieldBounds() const;
	sf::FloatRect			getMovementsfieldBounds() const;

	void					destroyEntitiesOutsideView();
	void					checkForCollision();
	void					handleCollisions();

	void					enemyProjectileCollision();
	void					playerProjectileCollision();
	void					enemyCollision();

	void					updateSounds();


private:
	sf::RenderTarget&		mTarget;
	FontHolder&				mFonts;
	SoundPlayer&			mSounds;

	sf::View				mWorldView;
	ImageHolder				mImages;
	TextureHolder			mTextures;

	SceneNode				mSceneGraph;
	LayerContainer			mSceneLayers;
	CommandQueue			mCommandQueue;

	sf::FloatRect			mWorldBounds;
	sf::Vector2f			mSpawnPosition;

	BossFactory				mBossFactory;
	PlayerFactory			mPlayerFactory;

	Player*					mPlayer;
	Boss*					mBoss;
	LifeNode*				mLife;
	ScoreNode*				mScore;

	QuadTree				mQuadTreePrimary;
	QuadTree				mQuadTreeSecondary;

	NodeContainer			mEnemyNodes;
	NodeContainer			mPlayerBulletNodes;
	NodeContainer			mEnemyBulletNodes;

	InvaderController		mInvaderController;

	std::size_t				mLivesCount;

	float					mDeadLine;
	bool					mIsGameEnded;
};
