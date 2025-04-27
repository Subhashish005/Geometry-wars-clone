#pragma once

#include "Entity.h"
#include "Vec2.h"
#include "EntityManager.h"

struct PlayerConfig { int SR, CR, FR, FB, FG, OR, OG, OB, OT, V; float S; };
struct EnemyConfig	{ int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FB, FG, OR, OG, OB, OT, V, L; float S; };

// SR = shape radius
// CR = collision radius
// S = speed
// FR,FG,FB = fill colors
// OR,OG,OB = outline colors
// OT = outline thickness
// V = vertices
// VMIN/VMAX = min/max vertices
// SMIN/SMAX = min/max speed
// L = lifespan(in frames)
// SI = spawn interval

class Game {
private:
	sf::RenderWindow	m_window;			// the main window to draw upon
	bool				m_paused = false;	// whether the game logic is going to be updated
	bool				m_running = true;	// is the game paused?
	EntityManager		m_entityManager;	// vector of all entities
	sf::Font			m_font;
	sf::Text			m_text;				// a sample ready to give any text related info (used as score)
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletConfig;
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnFrame = 0;
	int					m_spawnInterval;
	int					m_playerLives = 0;
	int					m_enemyKilled = 0;
	int					m_maxEnemySpawn = 0;
	sf::Clock			m_deltaClock;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);	// the path to config file
	void initWindow(const std::string& config, sf::RenderWindow &window);
	void initFont(const std::string& config, sf::Font &font);

	void setPaused(bool paused);			// use to pause the game
	
	void sMovement();						// System: entity Movement / position update
	void sPlayerMovement();
	void sLifeSpan();						// System: lifeSpan
	void sUserInput();						// System: handle user inputs
	void sRender();							// System: renders the game
	void sEnemySpawner();					// System: spawn enemies
	void sCollision();						// System: check if a collision occured and resolve it
	void sUpdatePlayerLives();				// System: increases player's live if certain number of enemies are killed
	void sGUI();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> m_player, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity>, const Vec2& mousePos);

	void endGameScreen();

public:
	Game(const std::string& config);		// constructor, takes in game config

	void run();
};