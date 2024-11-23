#include "Game.h"
#include"bullet1.h"
#include <cmath> // For atan2 and other math functions
#include "font2.h"
#include<fstream>
#include<filesystem>
#include<iostream>
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
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	this->player = new Player();
	this->player->setpos(desktopMode.width / 2.f - this->player->getBounds().width, desktopMode.height / 2.f + this->player->getBounds().height - 100.f);

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

	if (!this->killSoundBuffer.loadFromFile("Sound/kill.mp3")) {
		// Handle error loading sound
		std::cout << "Error loading kill sound!" << std::endl;
	}
	if (!this->pausesoundbuffer.loadFromFile("Sound/pause.mp3")) {
		// Handle error loading sound
		std::cout << "Error loading pause sound!" << std::endl;
	}
	this->killSound.setBuffer(this->killSoundBuffer);
	this->killSound.setVolume(50.f);
	this->pausesound.setBuffer(this->pausesoundbuffer);
	this->pausesound.setVolume(50.f);
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
			case GameState::Name_Input:
				this->getPlayerNameInput();
				break;

			case GameState::Pause:
				handlePauseevents(event);
				break;

			case GameState::GAME_OVER:
				handleGameOverEvents(event);
				break;

			case GameState::SCORES:
				handlescoreevents(event);
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
			if (ui)
			{
				ui->load_menu();
				ui->update();
			}
			break;

		case GameState::Credits:
			if (ui) ui->load_credits();
			ui->update();
			break;

		case GameState::INSTRUCTIONS:
			if (ui) ui->load_instructions();
			this->ui->update();
			break;

		case GameState::GAME:
			std::cerr << this->getlives();

			this->render();

			this->update();

			std::cerr << this->getlives();
			break;

		case GameState::Pause:
				this->ui->pause_page();
				this->ui->update();
				break;

		case GameState::GAME_OVER:
			this->saveScore();
			if (ui) ui->endgame();
			this->ui->update();
			break;

		case GameState::SCORES:
			this->displayHighScore();
			if (ui) 
			{
			
				this->window->draw(this->ui->back_button_outline);
				this->window->draw(this->ui->back_button);
				this->ui->update();
			}
				
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
	this->resetgame();
	if (this->game_state != GameState::MENU) {
		this->ui->update(); // Refresh UI elements
		this->ui->load_menu();
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	
		if (this->ui->play_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();

			

				if (playerName.isEmpty())  // Only ask for name if it's not set
				{
					game_state = GameState::Name_Input;
				}
				else
				{
					game_state = GameState::GAME;
				}

			
		}
		else if (this->ui->instructions_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();
			std::cout << "Instructions Button Clicked \n";
			prev_state = GameState::MENU;

			game_state = GameState::INSTRUCTIONS;
		}
		else if (this->ui->scores_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();

			std::cout << "Scores Button Clicked \n";
			game_state = GameState::SCORES;
		}
		else if (this->ui->credits_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();

			std::cout << "Credits Button Clicked \n";
			game_state = GameState::Credits;
		}
		else if (this->ui->Close_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();

			window->close();
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
			this->ui->playClickSound();
			if (prev_state == GameState::MENU)
			{
				std::cout << "Back Button Pressed \n";
				game_state = GameState::MENU;
			}
			else if (prev_state == GameState::Pause)
			{
				game_state = GameState::Pause;
			}
		}
	}
}

void Game::handleGameEvents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		pausesound.play();
		std::cout << "Pause / Exit Pressed \n";
		
		game_state = GameState::Pause;

	}
}

void Game::handlePauseevents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		this->pausesound.play();
		std::cout << "Pause / Exit Pressed \n";

		game_state = GameState::GAME;

	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();
			std::cout << "Back Button Pressed \n";
			game_state = GameState::GAME;
		}

		else if (this->ui->resume.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();
			std::cout << "Back Button Pressed \n";

			game_state = GameState::MENU;
		}
		else if (this->ui->Restart.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();
			std::cout << "Restart Button Pressed \n";
			resetgame();
			game_state = GameState::GAME;
		}

		else if (this->ui->Instructions.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			this->ui->playClickSound();

			prev_state = GameState::Pause;
			game_state = GameState::INSTRUCTIONS;
		}
		else if (this->ui->exit.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
			{
			this->ui->playClickSound();

				std::cout << "exit Button Pressed \n";
				window->close();

			}

	}
}





void Game::handleGameOverEvents(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			std::cout << "Back Button Clicked \n";
			this->ui->playClickSound();

			this->resetgame();
			game_state = GameState::MENU;
		}
		if (this->ui->Replay.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			std::cout << "Back Button Clicked \n";
			this->ui->playClickSound();

			this->resetgame();
			game_state = GameState::GAME;
		}
		if (this->ui->Close.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			std::cout << "Close Button Clicked \n";
			this->ui->playClickSound();

			window->close(); // Close the game window			
		}

	}
}

void Game::handlescoreevents(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (this->ui->back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			std::cout << "Back Button Pressed \n";
			this->ui->playClickSound();

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
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	this->lives -= 1;  // Decrease player lives

	if (this->lives != 0)
		this->player->resetstats();  // Reset health to max

	if (this->lives == 0)
		this->game_state = GameState::GAME_OVER;

	// Calculate bottom center position
	float posX = desktopMode.width / 2.f - this->player->getBounds().width ; // Center horizontally
	float posY = desktopMode.height - this->player->getBounds().height - 100.f;    // Position slightly above the bottom

	// Set player's position to the bottom center
	this->player->setpos(posX, posY);
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
		else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F11)
		{
			std::cout << "Key pressed: " << e.key.code << std::endl;
			std::cout << "F key pressed, toggling fullscreen.\n";
			handleResize();
		
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
		this->bullets.push_back(new Bullet(this->textures["BULLET"], this->player->getPos().x+this->player->getBounds().width/2.5f+5, this->player->getPos().y, 0.f, -1.f, 3.f * level, 2 * level, this->level));
		
	}
	this->player->update(this->level);

}


// Member variable to keep track of occupied positions
std::vector<float> occupiedPositions;
void Game::updateEnemies() {
	// Adjust spawn rate based on level
	this->spawnTimer += 0.5f * this->level;
	this->spawnTimermax = std::max(2.f, 50.f - (level * 2.f));

	// Regular enemy spawning logic (only for levels below 5)
	if (this->level < 5) {
		if (this->spawnTimer >= this->spawnTimermax) {
			float playerX = this->player->getPos().x;
			float randomX;
			bool validPosition = false;
			int attempts = 0;               // Track attempts to find a valid spawn position
			const int maxAttempts = 200;    // Increased attempts to ensure spawn success

			sf::Vector2u windowSize = this->window->getSize(); // Get current window size

			while (!validPosition && attempts < maxAttempts) {
				// Generate a random X coordinate around the player, within window bounds
				randomX = playerX + static_cast<float>(rand() % 401 - 200);
				randomX = std::max(0.f, std::min(randomX, static_cast<float>(windowSize.x - 50))); // Ensure within window bounds

				validPosition = true;

				// Ensure minimum distance from other enemies and no overlap with existing positions
				for (float occupiedX : occupiedPositions) {
					if (std::abs(occupiedX - randomX) < 30.f) { // Reduced minimum distance
						validPosition = false;
						break;
					}
				}

				++attempts;
			}

			if (validPosition) {
				// Spawn the enemy at a valid position
				this->enemies.push_back(new Enemy(randomX, -50.f, this->level, false));
				occupiedPositions.push_back(randomX); // Add the new spawn position to the occupied list
				this->spawnTimer = 0.f; // Reset spawn timer
				std::cout << "Enemy spawned at: " << randomX << std::endl;
			}
			else {
				std::cout << "Failed to spawn enemy after " << maxAttempts << " attempts.\n";
			}
		}
	}
	float centerX = static_cast<float>(this->window->getSize().x) / 2;

	// Boss spawning logic (only for levels 5 and above)
	if (this->level >= 5 && this->boss.empty()) {
		int bossesToSpawn = this->level - 4;

		// Adjust for multiple bosses, spacing them evenly around the center
		for (int i = 0; i < bossesToSpawn; ++i) {
			// Calculate offset for each boss based on the number of bosses to spawn
			float offset = (i - (bossesToSpawn - 1) / 2.f) * 150.f; // Spacing of 150 units between bosses
			float spawnX = centerX + offset;
			this->boss.push_back(new Enemy(spawnX, -200.f, this->level, true));
		}
		bossSpawnedCount = bossesToSpawn;
	}

	// Update regular enemies
	for (size_t i = 0; i < this->enemies.size(); ++i) {
		this->enemies[i]->update(this->level);
		this->enemies[i]->updateattack(this->getlevel());

		// Remove enemy if out of bounds
		if (this->enemies[i]->getBounds(false).top > this->window->getSize().y) {
			// Remove the corresponding position from occupiedPositions
			occupiedPositions.erase(
				std::remove(occupiedPositions.begin(), occupiedPositions.end(), this->enemies[i]->getPos(false).x),
				occupiedPositions.end()
			);

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

	// Update bosses
	for (size_t i = 0; i < this->boss.size(); ++i) {
		Enemy* boss = this->boss[i];
		float targetHeight = 20.f;

		if (boss->getPos(true).y < targetHeight) {
			boss->update(true);
		}
			// Boss shooting logic
			boss->updateBossAttack();
			if (boss->bosscanShoot()) {
				float shooterOffsetX = 20.f; // Adjust as needed
				float shooterY = boss->getPos(true).y + boss->getBounds(true).height;

				// Shooter positions (left and right of the boss)
				float leftShooterX = boss->getPos(true).x + shooterOffsetX;
				float rightShooterX = boss->getPos(true).x + boss->getBounds(true).width - shooterOffsetX;

				this->bossbullets.push_back(new Bullet(
					this->textures["BULLET"],
					leftShooterX,
					shooterY,
					0.f, 4.f, 5.f, 20.f, this->level // Adjust velocity/damage as needed
				));
				this->bossbullets.push_back(new Bullet(
					this->textures["BULLET"],
					rightShooterX,
					shooterY,
					0.f, 4.f, 5.f, 20.f, this->level // Adjust velocity/damage as needed
				));
			}

			// Check if the boss is destroyed
			if (boss->isDestroyed()) {
				delete boss;
				this->boss.erase(this->boss.begin() + i);
				--i;

				// Spawn additional bosses
				if (this->boss.empty() && bossSpawnedCount < this->level - 4) {
					bossSpawnedCount++;
					for (int j = 0; j < bossSpawnedCount; ++j) {
						// Recalculate the spawn positions for additional bosses
						float offset = (j - (bossSpawnedCount - 1) / 2.f) * 150.f;
						float spawnX = centerX + offset;
						this->boss.push_back(new Enemy(spawnX, -200.f, this->level, true));
					}

					// Clear bullets when new wave of bosses spawn
					for (size_t j = 0; j < this->enemyBullets.size(); ++j) {
						delete this->enemyBullets[j];
					}
					this->enemyBullets.clear();
				}
			}
		}
	}



void Game::updateEnemyBullets() {
	// Update all enemy bullets
	for (size_t i = 0; i < this->enemyBullets.size(); ++i) {
		this->enemyBullets[i]->update();

		// Check collision with player
		if (this->enemyBullets[i]->getBounds().intersects(this->player->getBounds())) {
			this->player->losehp(this->enemyBullets[i]->getdamage());
			if (this->player->getHp() <= 0) {
				this->killSound.play(); // Play kill sound if the player dies
			}
			delete this->enemyBullets[i];
			this->enemyBullets.erase(this->enemyBullets.begin() + i);
			--i;
			continue;
		}

		// Remove if off-screen
		if (this->enemyBullets[i]->getBounds().top > this->window->getSize().y) {
			delete this->enemyBullets[i];
			this->enemyBullets.erase(this->enemyBullets.begin() + i);
			--i;
		}
	}

	// Update all boss bullets
	for (size_t i = 0; i < this->bossbullets.size(); ++i) {
		this->bossbullets[i]->update();

		// Check collision with player
		if (this->bossbullets[i]->getBounds().intersects(this->player->getBounds())) {
			this->player->losehp(this->bossbullets[i]->getdamage());
			delete this->bossbullets[i];
			this->bossbullets.erase(this->bossbullets.begin() + i);
			--i;
			continue;
		}

		// Remove if off-screen
		if (this->bossbullets[i]->getBounds().top > this->window->getSize().y) {
			delete this->bossbullets[i];
			this->bossbullets.erase(this->bossbullets.begin() + i);
			--i;
		}
	}
}
void Game::updateEnemiesCombat() {
    // Handle regular enemies (only for levels below 5)
    if (this->level < 5) {
        for (size_t i = 0; i < this->enemies.size();) {
            bool enemy_removed = false;

            // Check collision with player bullets
            for (size_t k = 0; k < this->bullets.size() && !enemy_removed; ++k) {
                if (this->bullets[k]->getBounds().intersects(this->enemies[i]->getBounds(false))) {
                    // Player's bullet hits the enemy, gain points
                    this->points += this->enemies[i]->getPoints();
                    this->killSound.play();

                    // Delete the bullet and the enemy
                    delete this->bullets[k];
                    this->bullets.erase(this->bullets.begin() + k);

                    delete this->enemies[i];
                    this->enemies.erase(this->enemies.begin() + i);

                    enemy_removed = true;
                    break;
                }
            }

            // Check collision with player
            if (!enemy_removed && this->enemies[i]->getBounds(false).intersects(this->player->getBounds())) {
                // Player collides with enemy, player loses health
                this->player->losehp(this->enemies[i]->getDamage());
                this->killSound.play();

                // Remove the enemy
                delete this->enemies[i];
                this->enemies.erase(this->enemies.begin() + i);
                enemy_removed = true;
            }

            if (!enemy_removed) {
                ++i;
            }
        }
    }

    // Handle all bosses (only for levels 5 and above)
    if (this->level >= 5) {
        for (size_t j = 0; j < this->boss.size();) {
            Enemy* boss = this->boss[j];

            // Check collision with player bullets
            for (size_t k = 0; k < this->bullets.size(); ++k) {
                if (this->bullets[k]->getBounds().intersects(boss->getBounds(true))) {
                    // Bullet hits the boss, apply damage to the boss
                    boss->takeDamage(this->player->give_damage());
                    delete this->bullets[k];
                    this->bullets.erase(this->bullets.begin() + k);
                    break;
                }
            }

            // Check collision with the player
            if (boss->getBounds(true).intersects(this->player->getBounds())) {
                // Player collides with boss, player loses health
                this->player->losehp(boss->getDamage());
            }

            // Remove boss if defeated
            if (boss->gethp() <= 0) {
                this->points += boss->getPoints();
                this->killSound.play();

                delete boss;
                this->boss.erase(this->boss.begin() + j);

                // Increment boss wave count
                this->bossSpawnedCount++;
            }
            else {
                ++j;
            }
        }
    }

    // Check for collision between bullets and enemy/boss bullets
    for (size_t i = 0; i < this->bullets.size(); ++i) {
        for (size_t j = 0; j < this->enemyBullets.size(); ++j) {
            if (this->bullets[i]->getBounds().intersects(this->enemyBullets[j]->getBounds())) {
                // Bullet from the player hits enemy bullet
                delete this->bullets[i];
                this->bullets.erase(this->bullets.begin() + i);
                delete this->enemyBullets[j];
                this->enemyBullets.erase(this->enemyBullets.begin() + j);

                // Adjust indices after erasing
                i--; // Ensure the next bullet checks correctly
                break; // No need to check more collisions for the current bullet
            }
        }

        for (size_t j = 0; j < this->bossbullets.size(); ++j) {
            if (this->bullets[i]->getBounds().intersects(this->bossbullets[j]->getBounds())) {
                // Bullet from the player hits boss bullet
                delete this->bullets[i];
                this->bullets.erase(this->bullets.begin() + i);
                delete this->bossbullets[j];
                this->bossbullets.erase(this->bossbullets.begin() + j);

                // Adjust indices after erasing
                i--; // Ensure the next bullet checks correctly
                break; // No need to check more collisions for the current bullet
            }
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
	// Update hearts only during gameplay
	if (game_state == GameState::GAME && this->player->getHp() == 0) {
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

void Game::saveScore() {
	if (scoreSaved) return;  // Prevent saving multiple times
	scoreSaved = true;

	std::filesystem::path exePath = std::filesystem::current_path();
	std::filesystem::path scoreFilePath = exePath / "highscores.txt";  // Append the filename

	std::ofstream file(scoreFilePath, std::ios::app);
	if (file.is_open()) {
		file << this->playerName.toAnsiString() << ": " << this->getpoints() << "\n";  // Save name and score
		file.close();
	}
}

void Game::displayHighScore() {
	// Determine the file path relative to the executable
	std::filesystem::path exePath = std::filesystem::current_path();
	std::filesystem::path scoreFilePath = exePath / "highscores.txt";

	std::ifstream file(scoreFilePath);
	std::vector<std::pair<std::string, int>> scores;

	std::string line;
	// Read scores from the file
	if (file.is_open()) {
		while (std::getline(file, line)) {
			size_t delimiterPos = line.find(": ");
			if (delimiterPos != std::string::npos) {
				std::string name = line.substr(0, delimiterPos);
				int score = std::stoi(line.substr(delimiterPos + 2));
				scores.push_back({ name, score });
			}
		}
		file.close();
	}

	// Sort scores in descending order
	std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;  // Sort by score (highest first)
		});

	// Display top 3 scores or a default message if no scores exist
	std::string topScoresText = "High Scores:\n";
	std::string score;
	if (scores.empty()) {
		topScoresText = "No high scores yet!";
	}
	else {
		for (size_t i = 0; i < std::min<size_t>(3, scores.size()); ++i) {
			score += "->"+scores[i].first + ": " + std::to_string(scores[i].second) + "\n";
		}
	}

	// Update and draw the high scores text
	this->highScoreText.setString(topScoresText);
	this->highScoreText.setFont(this->font2);  // Ensure font is set
	this->highScoreText.setCharacterSize(60);
	this->highScoreText.setFillColor(sf::Color::Red);
	this->highScoreText.setPosition(this->window->getSize().x / 2.f-this->highScoreText.getGlobalBounds().width, this->window->getSize().y / 4.f);


	this->scores.setString(score);
	this->scores.setFont(this->font2);  // Ensure font is set
	this->scores.setCharacterSize(30);
	this->scores.setFillColor(sf::Color::White);
	this->scores.setPosition(this->highScoreText.getPosition().x, this->highScoreText.getPosition().y + this->highScoreText.getGlobalBounds().height+10.f);

	this->window->draw(this->highScoreText);
	this->window->draw(this->scores);
}


void Game::getPlayerNameInput() {
	sf::Text usernamePrompt;
	usernamePrompt.setFont(this->font2);
	usernamePrompt.setString("Enter your name:");
	usernamePrompt.setCharacterSize(50);
	usernamePrompt.setFillColor(sf::Color::Green);
	usernamePrompt.setPosition(this->window->getSize().x / 2.f - usernamePrompt.getLocalBounds().width / 2.f,
		this->window->getSize().y / 3.f);

	sf::Text usernameInput;
	usernameInput.setFont(this->font2);
	usernameInput.setCharacterSize(40);
	usernameInput.setFillColor(sf::Color::Yellow);
	usernameInput.setPosition(usernamePrompt.getPosition().x - usernameInput.getLocalBounds().width,
		this->window->getSize().y / 3.f + 60.f); // Slightly below the prompt

	std::string username;

	while (this->window->isOpen()) {
		sf::Event event;
		while (this->window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window->close();
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b' && !username.empty()) {  // Handle backspace
					username.pop_back();
				}
				else if (event.text.unicode < 128 && event.text.unicode != '\b') {  // Handle ASCII input
					username += static_cast<char>(event.text.unicode);
				}
				// Update the text object with the new username
				usernameInput.setString(username);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				std::cerr << this->getlives();
				if (!username.empty()) {  // Proceed only if a name is entered
					this->playerName = username;  // Store the player's name
					game_state = GameState::GAME;  // Switch to the game state

					// Clear Enter key state
					while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						// Do nothing, just wait for the key to be released
					}

					return;  // Exit the input loop
				}
			}
		}

		// Clear and redraw the window each frame
		this->window->clear();
		this->updateStars();
		this->window->draw(usernamePrompt);
		this->window->draw(usernameInput);
		this->window->display();
	}
}





void Game::update()
{
	this->updatePollEvents();  // Handle events (input, window events)

	this->ui->update();  // Update the UI (this might handle player score, health, etc.)
	this->updateStars();  // Update background stars or other environmental effects

	// Only update game logic if lives > 0 and game state is GAME
	if (this->getlives() != 0 && game_state == GameState::GAME) {

		this->updateInput();  // Update player input (e.g., movement, shooting)
		this->player->updateAttack(this->getlevel());  // Update player attacks

		this->updateBullets();  // Update bullets (move, check collisions)
		this->updateEnemies();  // Update enemies (move, spawn, etc.)
		this->updateEnemiesCombat();  // Check enemy and player collisions

		this->updateEnemyBullets();  // Update enemy bullets (movement, collision, etc.)

		this->updateUI();  // Update the user interface (score, health, etc.)

		// Update explosions after all the game mechanics
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
	for (auto* bullet : this->bossbullets)
		bullet->render(this->window);


    // Render UI components like health bars, score, and other details
    this->renderUI();
}


void Game::resetgame() {
	window->clear();
	this->player->resetstats();  // Reset player stats like health
	this->player->setpos(this->window->getSize().x / 2.f-this->player->getBounds().width, this->window->getSize().y -this->player->getBounds().height - 20.f);
	this->level = 1;
	this->lives = 3;
	this->points = 0;
	this->bossSpawned = false;
	this->scoreSaved = false;  // Reset for the next game
	this->player->sethpmax();
	// Clear existing game objects
	this->bullets.clear();
	this->enemyBullets.clear();
	this->enemies.clear();
	this->boss.clear();
	this->bossbullets.clear();

}
