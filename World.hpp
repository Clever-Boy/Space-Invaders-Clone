#pragma once


#include "SpriteNode.hpp"
#include "Spaceship.hpp"
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
	explicit							World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);

	void								update(sf::Time dt);
	void								draw();
	bool 								hasAlivePlayer() const;
	bool								hasPlayerWon() const;
	CommandQueue&						getCommandQueue();


private:
	void								loadTextures();
	void								buildScene();
	void								adaptPlayerPosition();

	void								addEnemies();
	void								addEnemy(Spaceship::Type type, float relX, float relY);

	sf::FloatRect						getViewBounds() const;

	void								destroyEntitiesOutsideView();
	void								handleCollisions();
	void								checkForCollision();

	void								controlEnemyFire();

	void								addLifes();
	void								addLife(float relX, float relY);


	void								addShields();
	void								adaptShieldPlaces(sf::Vector2f position);
	void								addShield(Shield::Type type, float relX, float relY);

	void								updateText();

	sf::FloatRect						getBattlefieldBounds() const;

	void								updateSounds();


private:
	enum Layer
	{
		Background,
		Space,
		LayerCount
	};


private:
	sf::RenderTarget&						mTarget;
	FontHolder&								mFonts;

	sf::View								mWorldView;
	TextureHolder							mTextures;

	SceneNode								mSceneGraph;
	std::array<SceneNode*, LayerCount>		mSceneLayers;
	CommandQueue							mCommandQueue;

	sf::FloatRect							mWorldBounds;
	sf::Vector2f							mSpawnPosition;

	Spaceship*								mPlayerShip;


	QuadTree								mQuadTreePrimary;
	QuadTree								mQuadTreeSecondary;
	std::vector<SceneNode*>					mEnemyNodes;
	std::vector<SceneNode*>					mPlayerBulletNodes;
	std::vector<SceneNode*>					mEnemyBulletNodes;

	std::vector<Spaceship*>					mActiveEnemies;
	std::list<std::unique_ptr<Life>>		mLives;

	float									mDeadLine;
	std::size_t								mScore;
	sf::Text								mScoreText;
	sf::Text								mStaticScoreText;
	sf::Text								mLivesText;
	SoundPlayer&							mSounds;
};
