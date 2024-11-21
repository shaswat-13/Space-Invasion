#include "Game.h"
#include"bullet1.h"
#include <cmath> // For atan2 and other math functions
#include "font2.h"

// Initialization functions...



void Game::initwindow()
{
	this->window = new sf::RenderWindow(
		sf::VideoMode::getFullscreenModes()[0], // Best fullscreen resolution
		"Space Invasion",
		sf::Style::Fullscreen // Start in fullscreen mode
	);

	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(true);
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initStars()
{
	srand(static_cast<unsigned>(time(0))); // Seed random number generator

	int numStars = static_cast<int>((this->window->getSize().x * this->window->getSize().y) / 50000); // Example: 1 star per 50,000 pixels
	for (int i = 0; i < numStars; ++i)
	{
		float x = static_cast<float>(rand() % this->window->getSize().x);
		float y = static_cast<float>(rand() % this->window->getSize().y);
		float radius = static_cast<float>(rand() % 3 + 1); // Radius between 1 and 3

		this->stars.push_back(new Star(x, y, radius));
	}
}

void Game::initEnemies()
{
	this->spawnTimer = 0.f;
	this->spawnTimermax = std::max(10.f, 50.f - (level * 2.f)); // Avoid spawn times below 10
}

void Game::initUI()
{
	this->ui = new UI(*this->window);  // Pass the window reference to UI


}



void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	this->textures["BULLET"]->loadFromMemory(bullet_png, bullet_png_len);
}

void Game::initSystems()
{
	this->points = 0;
}

Game::Game()

{
	this->game_state = GameState::MENU;
	if (!this->font2.loadFromMemory(font2_ttf, font2_ttf_len)) {
		std::cout << "Error: Font not loaded\n";
	}

	this->updatePollEvents();
	this->initwindow();
	this->initUI();

	this->initStars();
	this->initTextures();
	this->initPlayer();
	this->initEnemies();
	this->initSystems();
	
}

Game::~Game()
{
	delete this->window;
	delete this->player;
	delete this->ui;

	for (auto& i : this->textures)
		delete i.second;

	for (auto* i : this->bullets)
		delete i;

	for (auto* i : this->enemies)
		delete i;
	for (auto* i : this->boss)
		delete i;
	for (auto* i : this->stars)
		delete i;
}

// Main game functions...

void Game::run()
{
	sf::Event event;
	while (this->window && this->window->isOpen())
	{
		while (window->pollEvent(event))
		{

			// Render stars (background)
			for (auto* star : this->stars)
			{
				if (!star)
				{
					std::cerr << "Null star detected!" << std::endl;
					continue;
				}
				star->render(*this->window);
			}

			if (event.type == sf::Event::Closed)
			{
				std::cout << "Window closed \n";
				window->close();
			}

			switch (game_state)
			{
			case GameState::MENU:
				handleMenuEvents(event);
				break;

			case GameState::INSTRUCTIONS:
				handleInstructionsEvents(event);
				break;

			case GameState::GAME:
				handleGameEvents(event);
				break;

			case GameState::END:
				handleEndEvents(event);
				break;

			case GameState::GAME_OVER:
				handleGameOverEvents(event);
				break;

			default:
				std::cerr << "Unknown game_state: " << static_cast<int>(game_state) << std::endl;
				break;
			}
		}

		// Clear the screen
		if (!window)
		{
			std::cerr << "Window is null during rendering!" << std::endl;
			return;
		}
		window->clear(sf::Color::Black);

		// Render stars
		this->updateStars();
		for (auto* star : this->stars)
		{
			if (!star)
				continue;
			star->render(*this->window);
		}

		// Render based on game state
		switch (game_state)
		{
		case GameState::MENU:
			if (ui) ui->load_menu();
			break;

		case GameState::INSTRUCTIONS:
			if (ui) ui->load_instructions();
			break;

		case GameState::GAME:
			this->update();
			this->render();
			break;

		case GameState::END:
			if (ui) ui->endgame();
			break;

		case GameState::GAME_OVER:
			if (ui) ui->endgame();
			if (ui) window->draw(this->ui->back_button);
			break;

		default:
			std::cerr << "Rendering default menu for unknown game_state!" << std::endl;
			if (ui) ui->load_menu();
			break;
		}

		window->display();
	}
}

void Game::handleMenuEvents(sf::Event& event)
{
	if (this->game_state != GameState::MENU) {
		this->ui->update(); // Refresh UI elements
		this->ui->load_menu();
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->play_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Play Button Clicked \n";
			this->resetgame(); // Reset game state and variables
			game_state = GameState::GAME;
		}
		else if (this->ui->instructions_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Instructions Button Clicked \n";
			game_state = GameState::INSTRUCTIONS;
		}
		else if (this->ui->scores_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Scores Button Clicked \n";
			game_state = GameState::SCORES;
		}
	}
}

void Game::handleInstructionsEvents(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Back Button Pressed \n";
			game_state = GameState::MENU;
		}
	}
}

void Game::handleGameEvents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		std::cout << "Pause / Exit Pressed \n";
		game_state = GameState::MENU;
	}
}

void Game::handleEndEvents(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Back to Menu \n";
			this->resetgame();
			game_state = GameState::MENU;
		}
	}
}
void Game::handleGameOverEvents(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			std::cout << "Back Button Clicked \n";
			this->resetgame();
			game_state = GameState::MENU;
		}
	}
}
		

	

int Game::getpoints()
{

	return this->points;

}

int Game::getlevel()
{
	return this->level;

}

void Game::updatelevel()
{
	this->level += 1;
	this->updateStars();


}
int Game::getlives()
{
	return this->lives;
}
void Game::updatelives() {
	this->lives -= 1;  // Decrease player lives
	this->player->sethpmax();  // Reset health to max
	// Move player to the center of the window
	this->player->setpos(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f);

}



void Game::updatePollEvents()
{
	sf::Event e;
	while (this->window && this->window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			this->window->close();
		}
		else if (e.type == sf::Event::KeyPressed)
		{
			std::cout << "Key pressed: " << e.key.code << std::endl;

			if (e.key.code == sf::Keyboard::F)
			{
			std::cout << "F key pressed, toggling fullscreen.\n";
			handleResize();
		    }
		}
		else if (e.type == sf::Event::Resized)
		{
			// Handle window resize (e.g., maximize button or manual resize)
			std::cout << "Window resized to: " << e.size.width << "x" << e.size.height << std::endl;
			handleWindowResize(e.size.width, e.size.height);
		}
	}
}


void Game::handleWindowResize(unsigned int width, unsigned int height)
{
	if (this->window)
	{
		// Update the viewport to match the new window size
		sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
		this->window->setView(sf::View(visibleArea));

		// Reinitialize any resources that depend on the new size
		this->initStars();       // Adjust stars for the new resolution
		this->ui->update();      // Update UI elements for the new resolution
	}
}

void Game::handleResize()
{
	static bool isFullScreen = true;

	// Close the current window
	if (this->window)
	{
		this->window->close();
		delete this->window;
		this->window = nullptr;
	}

	if (isFullScreen)
	{
		// Switch to windowed mode (1200x800)
		this->window = new sf::RenderWindow(
			sf::VideoMode(1200, 800),
			"Space Invasion",
			sf::Style::Resize | sf::Style::Close | sf::Style::Titlebar
		);
	}
	else
	{
		// Switch to fullscreen mode
		this->window = new sf::RenderWindow(
			sf::VideoMode::getFullscreenModes()[0],
			"Space Invasion",
			sf::Style::Fullscreen
		);
	}

	// Ensure the new window is active
	this->window->setActive(true);

	// Toggle the mode
	isFullScreen = !isFullScreen;

	// Reinitialize resources dependent on the window
	this->initStars();       // Adjust stars for the new resolution
	this->ui->update();      // Update UI elements for the new resolution
	this->ui->load_menu();   // Reload menu (if applicable)
}


	

	


void Game::updateeffects() {
	
}



void Game::updateInput()
{
	// Move player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->player->move(-1.f, 0.f, *this->window);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->player->move(1.f, 0.f, *this->window);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->player->move(0.f, -1.f, *this->window);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->player->move(0.f, 1.f, *this->window);

	// Player shooting
	if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && this->player->canAttack())
	{
		this->bullets.push_back(new Bullet(this->textures["BULLET"], this->player->getPos().x + 30, this->player->getPos().y, 0.f, -1.f, 3.f*level, 2*level,this->level));
		
	}
	this->player->update(this->level);

}


void Game::updateEnemies() {
	// Adjust spawn rate based on level
	this->spawnTimer += 0.2f * this->level;
	this->spawnTimermax = std::max(20.f, 50.f - (level * 2.f));

	if (level < 5) {
		// Spawn regular enemies
		if (this->spawnTimer >= this->spawnTimermax) {
			float playerX = this->player->getPos().x;
			float randomX;
			bool validPosition = false;
			int attempts = 0;               // Track attempts to find a valid spawn position
			const int maxAttempts = 100;    // Max attempts to avoid infinite loop

			sf::Vector2u windowSize = this->window->getSize(); // Get current window size

			while (!validPosition && attempts < maxAttempts) {
				// Generate a random X coordinate around the player, within window bounds
				randomX = playerX + static_cast<float>(rand() % 401 - 200);
				randomX = std::max(0.f, std::min(randomX, static_cast<float>(windowSize.x - 50))); // Ensure within window bounds

				validPosition = true;

				// Ensure minimum distance from other enemies
				for (const auto& enemy : this->enemies) {
					if (std::abs(enemy->getPos(false).x - randomX) < 50.f) {
						validPosition = false;
						break;
					}
				}

				++attempts;
			}

			if (validPosition) {
				// Spawn the enemy at a valid position
				this->enemies.push_back(new Enemy(randomX, -50.f, this->level, false));
				this->spawnTimer = 0.f;
			}
		}

		// Update enemies and check for removal
		for (size_t i = 0; i < this->enemies.size(); ++i) {
			this->enemies[i]->update(this->level);
			this->enemies[i]->updateattack(this->getlevel());

			// Remove enemy if out of bounds
			if (this->enemies[i]->getBounds(false).top > this->window->getSize().y) {
				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);
				--i;
				continue;
			}

			// Handle enemy shooting
			if (this->enemies[i]->canShoot()) {
				this->enemyBullets.push_back(new Bullet(
					this->textures["BULLET"],
					this->enemies[i]->getPos(false).x + 20,
					this->enemies[i]->getBounds(false).top + this->enemies[i]->getBounds(false).height,
					0.f, 1.f, 3.f * level, 2 * level, this->level
				)); // Downward bullet
			}
		}
	}

	// Spawn the boss at level 5
	if (this->level == 5 && !bossSpawned) {
		this->boss.push_back(new Enemy(this->window->getSize().x / 2.f - 70.f, -300.f, this->level, true));
		bossSpawned = true;
		// Delete all remaining bullets
		for (size_t i = 0; i < this->enemyBullets.size(); ++i) 
		{
			delete this->enemyBullets[i];
		}
		this->enemyBullets.clear();
	}

	// Update the boss if present
	if (!this->boss.empty()) {
		Enemy* boss = this->boss.front(); // Assuming only one boss
		float targetHeight = 20.f;       // The height where the boss should stop moving down

		if (boss->getPos(true).y < targetHeight) {
			boss->update(true); // Update the boss only if it hasn't reached the target height
		}
	}
}


void Game::updateEnemyBullets()
{
	if (this->level < 5)
	{
		for (size_t i = 0; i < this->enemyBullets.size(); ++i) {
			this->enemyBullets[i]->update();

			// Check collision with player
			if (this->enemyBullets[i]->getBounds().intersects(this->player->getBounds())) {
				this->player->losehp(enemyBullets[i]->getdamage());

				delete this->enemyBullets[i];
				this->enemyBullets.erase(this->enemyBullets.begin() + i);
				--i;
				continue;
			}

			// Check collision with player bullets
			for (size_t j = 0; j < this->bullets.size(); ++j) {
				if (this->enemyBullets[i]->getBounds().intersects(this->bullets[j]->getBounds())) {
					delete this->enemyBullets[i];
					delete this->bullets[j];
					this->enemyBullets.erase(this->enemyBullets.begin() + i);
					this->bullets.erase(this->bullets.begin() + j);
					--i;
					break;
				}
			}

			// Remove if off-screen
			if (i < this->enemyBullets.size() && this->enemyBullets[i]->getBounds().top > this->window->getSize().y) {
				delete this->enemyBullets[i];
				this->enemyBullets.erase(this->enemyBullets.begin() + i);
				--i;
			}
		}
	}
	else
	{


	}

}
void Game::updateEnemiesCombat()
{
	// Iterate over enemies
	for (size_t i = 0; i < this->enemies.size();)
	{
		bool enemy_removed = false;

		// Check collision with bullets for enemies
		for (size_t k = 0; k < this->bullets.size() && !enemy_removed; ++k)
		{
			if (this->bullets[k]->getBounds().intersects(this->enemies[i]->getBounds(false)))
			{
				std::cout << "Collision detected!" << std::endl;

				this->points += this->enemies[i]->getPoints();
				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);
				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);
				enemy_removed = true;
				break; // Exit inner loop
			}
		}

		// Check collision with player if enemy is still present
		if (!enemy_removed && this->enemies[i]->getBounds(false).intersects(this->player->getBounds()))
		{
			this->player->losehp(this->enemies[i]->getDamage());
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);
			enemy_removed = true;
		}

		// If the enemy was not removed, continue with the next one
		if (!enemy_removed) {
			++i;
		}
	}

	// Iterate over bosses
	for (size_t j = 0; j < this->boss.size();)
	{
		bool boss_removed = false;
		Enemy* boss = this->boss[j]; // Get the current boss

		// Check collision with bullets for boss
		for (size_t k = 0; k < this->bullets.size() && !boss_removed; ++k)
		{
			if (this->bullets[k]->getBounds().intersects(boss->getBounds(true)))
			{
				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);
				boss->takeDamage(this->player->give_damage());
				std::cout <<"damage" << this->player->give_damage();
			}
		}

		// Check collision with player if boss is still present
		if (!boss_removed && boss->getBounds(true).intersects(this->player->getBounds()))
		{
			this->player->losehp(boss->getDamage());
		}

		// If boss is defeated
		if (boss->gethp() == 0)
		{
			this->boss.erase(this->boss.begin() + j);
			delete boss;
			boss_removed = true;
		}

		// If the boss was not removed, continue with the next one
		if (!boss_removed) {
			++j;
		}
	}
}


void Game::updateBullets()
{
	for (size_t i = 0; i < this->bullets.size();)
	{
		this->bullets[i]->update();

		// Remove bullets if they move off the screen
		if (this->bullets[i]->getBounds().top + this->bullets[i]->getBounds().height <= 0.f)
		{
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void Game::updateStars()
{

	float levelSpeedMultiplier;
	levelSpeedMultiplier = std::min(3.f, 1.0f + this->level * 0.3f); // Minimum cooldown of 0.5s

	for (auto& star : this->stars) {
		star->update(levelSpeedMultiplier,*this->window);
	}
}

void Game::updateUI() {

	//game over
	if (this->getlives() == 0)
	{
		this->game_state = GameState::GAME_OVER;
	}
	
	//updatehearts
	if (this->player->getHp() == 0) 
	{
		this->updatelives();
	}
	float playerHealthPercent = (this->player->getHp() / this->player->getHpMax());

	this->ui->updateHearts(this->getlives(),playerHealthPercent);

	this->ui->updateHealthBar(playerHealthPercent, this->player->getPos().x + 16, this->player->getPos().y +120.f,sf::Color::Green);

	if (!this->boss.empty()) {
		Enemy* boss = this->boss.front();  // Assuming only one boss
		float bossHealthPercent = (boss->gethp() / boss->gethpmax());
		// Update the health bar of the boss
		this->ui->updatebossHealthBar(bossHealthPercent, boss->getPos(true).x + 43.f, boss->getPos(true).y, sf::Color::Red);  // Red for boss

	}

	// Dynamically position score based on window size
	sf::Vector2u windowSize = this->window->getSize();
	float scorePosX = windowSize.x * 0.05f; // 85% of the window width
	float scorePosY = windowSize.y * 0.10f; // 5% from the top of the window

	int score = this->getpoints();
	if (score > 5 * this->getlevel()) {
		this->updatelevel();
	}
	int level = this->getlevel();

	// Update the score and level position dynamically
	this->ui->updateScoreAndLevel(score, level, scorePosX, scorePosY);

	// Check if the player's HP is zero to end the game
	
}



void Game::update()
{
	this->updatePollEvents();

	this->ui->update();
	this->updateStars();

	if (this->getlives() != 0 && game_state == GameState::GAME) {
		
		this->updateInput();
		this->player->updateAttack(this->getlevel());
		
		this->updateBullets();
		this->updateEnemies();
		this->updateEnemiesCombat();
		this->updateEnemyBullets();
		this->updateUI();
	}
	


}

void Game::renderUI()
{
	this->ui->render(this->window);
}
void Game::render() {
    // Render player
    this->player->render(*this->window);

    // Render player bullets
    for (auto* bullet : this->bullets)
        bullet->render(this->window);

    // Render enemy minions
    for (auto* enemy : this->enemies)
        enemy->renderminion(this->window);

    // Render enemy bullets
    for (auto* bullet : this->enemyBullets)
        bullet->render(this->window);

    // Render boss if present
    if (!this->boss.empty()) {
        for (auto* b : this->boss)
            b->renderboss(this->window);
    }

    // Render UI components like health bars, score, and other details
    this->renderUI();
}


void Game::resetgame() {
	window->clear();
	this->player->resetstats();  // Reset player stats like health
	this->player->move(this->window->getSize().x / 2.f, this->window->getSize().y/2.f, *window);
	this->level = 1;
	this->lives = 3;
	this->points = 0;
	this->updateStars();
	this->bossSpawned = false;
	// Clear existing game objects
	this->bullets.clear();
	this->enemyBullets.clear();
	this->enemies.clear();
	this->boss.clear();
	this->ui->update();

}
