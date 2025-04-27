#include "Game.h"

Game::Game(const std::string& config) {
	init(config);
}

void Game::initWindow(const std::string& config, sf::RenderWindow &window) {
	std::ifstream fin(config);

	if (!fin.is_open()) {
		std::cerr << "couldn't open config file!\n";

		exit(-1);
	}
	
	std::string temp;
	uint16_t width;
	uint16_t height;
	uint16_t FPSLimit;
	bool fullscreen;

	while (fin >> temp) {
		if (temp == "Window") {
			fin >> width >> height >> FPSLimit >> fullscreen;
			sf::Text windowTitle("Game", m_font, m_text.getCharacterSize());
		
			if (fullscreen) {
				window.create(sf::VideoMode(width, height), windowTitle.getString(), sf::Style::Fullscreen);
			}
			else {
				window.create(sf::VideoMode(width, height), windowTitle.getString());
			}

			window.setFramerateLimit(FPSLimit);

			break;
		}
	}
}

void Game::initFont(const std::string& config, sf::Font& font) {
	std::ifstream fin(config);

	if (!fin.is_open()) {
		std::cerr << "couldn't open config file!\n";

		exit(-1);
	}

	std::string temp;
	std::string fontFilePath;
	uint16_t size;
	uint16_t colorR;
	uint16_t colorB;
	uint16_t colorG;

	while (fin >> temp) {
		if (temp == "Font") {
			fin >> fontFilePath;

			if (!font.loadFromFile(fontFilePath)) {
				std::cerr << "couldn't load font correctly!\n";

				exit(-1);
			}

			fin >> size >> colorR >> colorG >> colorB;

			m_text.setFont(font);
			m_text.setCharacterSize(size);
			m_text.setFillColor(sf::Color((sf::Uint8)colorR, (sf::Uint8)colorG, (sf::Uint8)colorB));

			break;
		}
	}
}

void Game::init(const std::string& config) {
	initFont(config, m_font);
	initWindow(config, m_window);
	
	std::ifstream fin(config);


	if (!fin.is_open()) {
		std::cerr << "couldn't open config file!\n";

		exit(-1);
	}

	std::string temp;

	while (fin >> temp) {
		if (temp == "Player") {
			fin >> m_playerConfig.SR
				>> m_playerConfig.CR
				>> m_playerConfig.S
				>> m_playerConfig.FR
				>> m_playerConfig.FG
				>> m_playerConfig.FB
				>> m_playerConfig.OR
				>> m_playerConfig.OB
				>> m_playerConfig.OG
				>> m_playerConfig.OT
				>> m_playerConfig.V;
		}

		if (temp == "Enemy") {
			fin >> m_enemyConfig.SR
				>> m_enemyConfig.CR
				>> m_enemyConfig.SMIN
				>> m_enemyConfig.SMAX
				>> m_enemyConfig.OR
				>> m_enemyConfig.OG
				>> m_enemyConfig.OB
				>> m_enemyConfig.OT
				>> m_enemyConfig.VMIN
				>> m_enemyConfig.VMAX
				>> m_enemyConfig.L
				>> m_enemyConfig.SI;
		}

		if (temp == "Bullet") {
			fin >> m_bulletConfig.SR
				>> m_bulletConfig.CR
				>> m_bulletConfig.S
				>> m_bulletConfig.FR
				>> m_bulletConfig.FG
				>> m_bulletConfig.FB
				>> m_bulletConfig.OR
				>> m_bulletConfig.OB
				>> m_bulletConfig.OG
				>> m_bulletConfig.OT
				>> m_bulletConfig.V
				>> m_bulletConfig.L;
		}
	}

	m_spawnInterval = m_enemyConfig.SI;
}

void Game::setPaused(bool paused) {
	m_paused = true;
}

void Game::spawnPlayer() {
	auto e = m_entityManager.addEntity("player");

	e->cTransform = std::make_shared<CTransform>(Vec2(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
	e->cShape = std::make_shared<CShape>(m_playerConfig.SR,
		m_playerConfig.V,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
		m_playerConfig.OT);
	e->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	e->cInput = std::make_shared<CInput>();

	e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
	
	m_player = e;
}

void Game::sMovement() {
	sPlayerMovement();

	// not passing by reference intentionally
	// movement logic for bullets

	for (auto bullet : m_entityManager.getEntities("bullet")) {
		if (bullet->isActive()) {
			bullet->cTransform->pos += bullet->cTransform->velocity;

			bullet->cShape->circle.setPosition(bullet->cTransform->pos.x, bullet->cTransform->pos.y);
		}
	}

	for (auto enemy : m_entityManager.getEntities("enemy")) {
		if (enemy->isActive()) {
			enemy->cTransform->pos += enemy->cTransform->velocity;

			enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
			enemy->cTransform->angle += 2.5f;
			enemy->cShape->circle.setRotation(enemy->cTransform->angle);
		}
	}

	for (auto enemy : m_entityManager.getEntities("small enemy")) {
		if (enemy->isActive()) {
			enemy->cTransform->pos += enemy->cTransform->velocity;

			enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
			enemy->cTransform->angle += 2.5f;
			enemy->cShape->circle.setRotation(enemy->cTransform->angle);
		}
	}
}

void Game::sPlayerMovement() {
	float angle = 0.0f;
	const float PI = 3.14159;

	if (m_player->cInput->up && !m_player->cInput->left && !m_player->cInput->down && m_player->cInput->right) {
		angle = PI / 4;
	}

	if (m_player->cInput->up && m_player->cInput->left && !m_player->cInput->down && !m_player->cInput->right) {
		angle = (3 * PI) / 4;
	}

	if (!m_player->cInput->up && m_player->cInput->left && m_player->cInput->down && !m_player->cInput->right) {
		angle = (5 * PI) / 4;
	}

	if (!m_player->cInput->up && !m_player->cInput->left && m_player->cInput->down && m_player->cInput->right) {
		angle = (7 * PI) / 4;
	}

	float argX = 1.0f, argY = 1.0f;

	if (angle) {
		argX = std::fabs(std::cosf(angle));
		argY = std::fabs(std::sinf(angle));
	}

	if (m_player->cShape->circle.getPosition().y > m_player->cCollision->radius + m_playerConfig.OT && m_player->cInput->up) {
		m_player->cTransform->pos.y -= m_player->cTransform->velocity.y * argY;
	}

	if (m_player->cShape->circle.getPosition().x > m_player->cCollision->radius + m_playerConfig.OT && m_player->cInput->left) {
		m_player->cTransform->pos.x -= m_player->cTransform->velocity.x * argX;
	}

	if (m_window.getSize().y - m_player->cShape->circle.getPosition().y > m_player->cCollision->radius + m_playerConfig.OT && m_player->cInput->down) {
		m_player->cTransform->pos.y += m_player->cTransform->velocity.y * argY;
	}

	if (m_window.getSize().x - m_player->cShape->circle.getPosition().x > m_player->cCollision->radius + m_playerConfig.OT && m_player->cInput->right) {
		m_player->cTransform->pos.x += m_player->cTransform->velocity.x * argX;
	}

	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cTransform->angle += 2.5f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);
}

void Game::sLifeSpan() {
	// bullet lifespan and fading logic

	for (auto bullet : m_entityManager.getEntities("bullet")) {
		(bullet->cLifespan->remaining)--;

		bullet->cShape->circle.setFillColor(sf::Color((sf::Uint8)m_bulletConfig.FR,
			(sf::Uint8)m_bulletConfig.FG,
			(sf::Uint8)m_bulletConfig.FB,
			(sf::Uint8)(((float)bullet->cLifespan->remaining / bullet->cLifespan->total) * 255u)));

		bullet->cShape->circle.setOutlineColor(sf::Color((sf::Uint8)m_bulletConfig.OR,
			(sf::Uint8)m_bulletConfig.OG,
			(sf::Uint8)m_bulletConfig.OB,
			(sf::Uint8)(((float)bullet->cLifespan->remaining / bullet->cLifespan->total) * 255u)));

		if (!(bullet->cLifespan->remaining)) {
			bullet->destroy();
		}
	}

	// small enemy lifespan and fading logic

	for (auto enemy : m_entityManager.getEntities("small enemy")) {
		(enemy->cLifespan->remaining)--;

		enemy->cShape->circle.setFillColor(sf::Color(enemy->cShape->circle.getFillColor().r,
			enemy->cShape->circle.getFillColor().g,
			enemy->cShape->circle.getFillColor().b,
			(sf::Uint8)(((float)enemy->cLifespan->remaining / enemy->cLifespan->total) * 255u)));

		enemy->cShape->circle.setOutlineColor(sf::Color(enemy->cShape->circle.getOutlineColor().r,
			enemy->cShape->circle.getOutlineColor().g,
			enemy->cShape->circle.getOutlineColor().b,
			(sf::Uint8)(((float)enemy->cLifespan->remaining / enemy->cLifespan->total) * 255u)));

		if (!(enemy->cLifespan->remaining)) {
			enemy->destroy();
		}
	}
}

void Game::sUserInput() {
	sf::Event event;

	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			m_window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			m_player->cInput->up = true;
		}
		else {
			m_player->cInput->up = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			m_player->cInput->left = true;
		}
		else {
			m_player->cInput->left = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			m_player->cInput->down = true;
		}
		else {
			m_player->cInput->down = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			m_player->cInput->right = true;
		}
		else {
			m_player->cInput->right = false;
		}

		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			m_player->cInput->shoot = true;
		}
	}
}

void sGUI() {}		// todo

void Game::sRender() {
	m_window.clear();

	m_text.setString("Score: " + std::to_string(m_score) + " | " + "remaining Lives: " + std::to_string(m_playerLives + 1));

	if (m_player->cInput->shoot) {
		spawnBullet(m_player, Vec2(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y));
	}

	m_player->cInput->shoot = false;

	for (auto e : m_entityManager.getEntities()) {
		if (e->isActive()) {
			m_window.draw(e->cShape->circle);
		}
	}

	m_window.draw(m_player->cShape->circle);
	m_window.draw(m_text);

	m_window.display();
	
	if (m_playerLives < 0) {
		m_text.setString("Score: " + std::to_string(m_score));
	}
}

void Game::sCollision() {
	// logic for enemy collision with wall

	for (auto enemy : m_entityManager.getEntities("enemy")) {
		if (enemy->cShape->circle.getPosition().y < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.y *= -1;
		}

		if (enemy->cShape->circle.getPosition().x < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.x *= -1;
		}

		if (m_window.getSize().y - enemy->cShape->circle.getPosition().y < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.y *= -1;
		}

		if (m_window.getSize().x - enemy->cShape->circle.getPosition().x < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.x *= -1;
		}
	}

	// logic for small enemy collision with wall

	for (auto enemy : m_entityManager.getEntities("small enemy")) {
		if (enemy->cShape->circle.getPosition().y < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.y *= -1;
		}

		if (enemy->cShape->circle.getPosition().x < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.x *= -1;
		}

		if (m_window.getSize().y - enemy->cShape->circle.getPosition().y < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.y *= -1;
		}

		if (m_window.getSize().x - enemy->cShape->circle.getPosition().x < enemy->cCollision->radius + m_enemyConfig.OT) {
			enemy->cTransform->velocity.x *= -1;
		}
	}

	// bullet and enemy collision logic

	for (auto bullet : m_entityManager.getEntities("bullet")) {
		for (auto enemy : m_entityManager.getEntities("enemy")) {
			if (((Vec2)bullet->cShape->circle.getPosition()).distSqr((Vec2)enemy->cShape->circle.getPosition()) < std::powf(m_bulletConfig.CR + m_enemyConfig.CR, 2)) {
				m_score += enemy->cScore->score;
				m_enemyKilled++;
				m_maxEnemySpawn--;

				if (m_enemyKilled == 25) {
					m_enemyKilled = 0;

					if(m_playerLives < 5)
						sUpdatePlayerLives();
				}

				bullet->destroy();
				enemy->destroy();
				spawnSmallEnemies(enemy);
			}
		}
	}

	// bullet and small enemy collision logic

	for (auto bullet : m_entityManager.getEntities("bullet")) {
		for (auto enemy : m_entityManager.getEntities("small enemy")) {
			if (((Vec2)bullet->cShape->circle.getPosition()).distSqr((Vec2)enemy->cShape->circle.getPosition()) < std::powf(m_bulletConfig.CR + m_enemyConfig.CR, 2)) {
				m_score += enemy->cScore->score;

				bullet->destroy();
				enemy->destroy();
			}
		}
	}

	// enemy and player collision logic

	for (auto enemy : m_entityManager.getEntities("enemy")) {
		if (((Vec2)m_player->cShape->circle.getPosition()).distSqr((Vec2)enemy->cShape->circle.getPosition()) < std::powf(m_playerConfig.CR + m_enemyConfig.CR, 2)) {
			m_playerLives--;
			m_maxEnemySpawn--;

			m_player->destroy();
			enemy->destroy();
			spawnSmallEnemies(enemy);

			spawnPlayer();
		}
	}

	// small enemy and player collision logic

	for (auto enemy : m_entityManager.getEntities("small enemy")) {
		if (((Vec2)m_player->cShape->circle.getPosition()).distSqr((Vec2)enemy->cShape->circle.getPosition()) < std::powf(m_playerConfig.CR + m_enemyConfig.CR, 2)) {
			m_playerLives--;

			m_player->destroy();
			enemy->destroy();

			spawnPlayer();
		}
	}
}

void Game::sEnemySpawner() {
	if (m_currentFrame - m_lastEnemySpawnFrame == m_spawnInterval) {
		if (m_maxEnemySpawn < 25) {
			m_maxEnemySpawn++;
			spawnEnemy();
		}

		m_lastEnemySpawnFrame = m_currentFrame;
	}
}

void Game::sUpdatePlayerLives() {
	m_playerLives++;
}

void Game::spawnEnemy() {
	auto e = m_entityManager.addEntity("enemy");

	std::srand(m_deltaClock.getElapsedTime().asMicroseconds());

	// TODO : make a safe zone around player
	// update : done

	float randPosX;
	float randPosY;
	int noSpawnDistance = 150;

	if ((std::rand() % 100) % 2) {
		if ((std::rand() % 100) % 2) {
			randPosX = std::rand() % ((int)m_player->cShape->circle.getPosition().x - (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance) - (m_enemyConfig.CR + m_enemyConfig.OT) + 1) + (m_enemyConfig.CR + m_enemyConfig.OT);
		}
		else {
			randPosX = std::rand() % (m_window.getSize().x - (m_enemyConfig.CR + m_enemyConfig.OT) - ((int)m_player->cShape->circle.getPosition().x + (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance)) + 1) + (int)m_player->cShape->circle.getPosition().x + (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance);
		}

		randPosY = std::rand() % (m_window.getSize().y - (m_enemyConfig.CR + m_enemyConfig.OT) - (m_enemyConfig.CR + m_enemyConfig.OT) + 1) + (m_enemyConfig.CR + m_enemyConfig.OT);
	}
	else {
		if ((std::rand() % 100) % 2) {
			randPosY = std::rand() % ((int)m_player->cShape->circle.getPosition().y - (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance) - (m_enemyConfig.CR + m_enemyConfig.OT) + 1) + (m_enemyConfig.CR + m_enemyConfig.OT);
		}
		else {
			randPosY = std::rand() % (m_window.getSize().y - (m_enemyConfig.CR + m_enemyConfig.OT) - ((int)m_player->cShape->circle.getPosition().y + (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance)) + 1) + (int)m_player->cShape->circle.getPosition().y + (m_playerConfig.CR + m_playerConfig.OT + noSpawnDistance);
		}

		randPosX = std::rand() % (m_window.getSize().x - (m_enemyConfig.CR + m_enemyConfig.OT) - (m_enemyConfig.CR + m_enemyConfig.OT) + 1) + (m_enemyConfig.CR + m_enemyConfig.OT);
	}
	

	float angle = std::atan2f(std::pow(-1, std::rand()) * randPosX - (m_enemyConfig.CR + m_playerConfig.OT),std::pow(-1, std::rand()) * randPosY - (m_enemyConfig.CR + m_playerConfig.OT));
	float randSpeedX = std::rand() % ((int)m_enemyConfig.SMAX - (int)m_enemyConfig.SMIN + 1) + m_enemyConfig.SMIN;
	float randSpeedY = std::rand() % ((int)m_enemyConfig.SMAX - (int)m_enemyConfig.SMIN + 1) + m_enemyConfig.SMIN;
	e->cTransform = std::make_shared<CTransform>(Vec2(randPosX, randPosY), Vec2(randSpeedX * std::cosf(angle), randSpeedY * std::sinf(angle)), 0.0f);

	float randVertices = std::rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1) + m_enemyConfig.VMIN;
	uint16_t randFillColorR = std::rand() % (255 - 50 + 1) + 50;
	uint16_t randFillColorG = std::rand() % (255 - 50 + 1) + 50;
	uint16_t randFillColorB = std::rand() % (255 - 50 + 1) + 50;
	uint16_t randOutlineColorR = std::rand() % (255 - 50 + 1) + 50;			// there are OR, OG, OB components in config file... lol
	uint16_t randOutlineColorG = std::rand() % (255 - 50 + 1) + 50;
	uint16_t randOutlineColorB = std::rand() % (255 - 50 + 1) + 50;

	e->cShape = std::make_shared<CShape>(m_enemyConfig.SR,
		randVertices,
		sf::Color((sf::Uint8)randFillColorR, (sf::Uint8)randFillColorG, (sf::Uint8)randFillColorB),
		sf::Color((sf::Uint8)randOutlineColorR, (sf::Uint8)randOutlineColorG, (sf::Uint8)randOutlineColorB),
		m_enemyConfig.OT);

	e->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	e->cScore = std::make_shared<CScore>(randVertices * 100);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {
	const float PI = 3.14159;
	float spawnAngle = 0.0f;
	float spawnAngleDiff = (360.0f / entity->cShape->circle.getPointCount()) * PI * 0.00555;
	
	for (uint16_t i = 0; i < entity->cShape->circle.getPointCount(); i++) {
		auto e = m_entityManager.addEntity("small enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2(entity->cShape->circle.getPosition()),
			Vec2(entity->cTransform->velocity.getMagnitude() * 0.5f * std::cosf(spawnAngle), entity->cTransform->velocity.getMagnitude() * 0.5f * std::sinf(spawnAngle)), 0.0f);
		e->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius() * 0.275f,
			entity->cShape->circle.getPointCount(),
			entity->cShape->circle.getFillColor(),
			entity->cShape->circle.getOutlineColor(),
			entity->cShape->circle.getOutlineThickness());
		e->cCollision = std::make_shared<CCollision>(entity->cCollision->radius * 0.275f);
		e->cScore = std::make_shared<CScore>(entity->cScore->score * 2);
		e->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);

		spawnAngle += spawnAngleDiff;
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> m_player, const Vec2& mousePos) {
	auto e = m_entityManager.addEntity("bullet");

	float vX = mousePos.x - m_player->cTransform->pos.x;
	float vY = mousePos.y - m_player->cTransform->pos.y;
	float angle = atan2f(vY, vX);

	e->cTransform = std::make_shared<CTransform>(Vec2(m_player->cTransform->pos.x, m_player->cTransform->pos.y), Vec2(m_bulletConfig.S * cosf(angle), m_bulletConfig.S * sinf(angle)), 0.0f);
	e->cShape = std::make_shared<CShape>(m_bulletConfig.SR,
		m_bulletConfig.V,
		sf::Color((sf::Uint8)m_bulletConfig.FR, (sf::Uint8)m_bulletConfig.FG, (sf::Uint8)m_bulletConfig.FB),
		sf::Color((sf::Uint8)m_bulletConfig.OR, (sf::Uint8)m_bulletConfig.OG, (sf::Uint8)m_bulletConfig.OB),
		m_bulletConfig.OT);
	e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);

	e->cShape->circle.setPosition(sf::Vector2f(e->cTransform->pos.x, e->cTransform->pos.y));
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& mousePos) {}		// todo

void Game::endGameScreen() {
	m_window.clear();

	sf::Text gameOver("GAME OVER", m_font, 50u);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setOrigin(gameOver.getLocalBounds().left + gameOver.getLocalBounds().width / 2.0f,
		gameOver.getLocalBounds().top + gameOver.getLocalBounds().height / 2.0f);
	gameOver.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - gameOver.getCharacterSize());

	//m_text.setString("Score: " + std::to_string(m_score) + "\npress esc to close this screen");
	m_text.setCharacterSize(40u);
	m_text.setPosition(m_window.getSize().x / 2.0f - m_text.getLocalBounds().width / 2.0f, m_window.getSize().y / 2.0f);

	m_window.draw(gameOver);
	m_window.draw(m_text);
	m_window.display();
}

void Game::run() {
	spawnPlayer();

	while (m_window.isOpen()) {
		if (m_playerLives < 0) {
			sUserInput();
			endGameScreen();
		}
		else {
			m_entityManager.update();

			sUserInput();
			sCollision();
			sMovement();
			sEnemySpawner();
			sLifeSpan();
			sRender();
		}

		m_currentFrame++;
	}

	m_window.~RenderWindow();
}