#include "Game.h"
#include"bullet1.h"
#include <cmath> // For atan2 and other math functions


// Initialization functions...

void Game::initwindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Invasion", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initStars()
{
	srand(static_cast<unsigned>(time(0))); // Seed random number generator

	int numStars = 100;
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
	this->initwindow();
	this->initStars();
	this->initTextures();
	this->initPlayer();
	this->initEnemies();
	this->initSystems();
	this->initUI();
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

	for (auto* i : this->stars)
		delete i;
}

// Main game functions...

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();

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

}

void Game::updatePollEvents()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed ||
			(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
			this->window->close();
	}
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


void Game::updateEnemies()
{
	this->spawnTimer += 0.2f * this->level; // Increase spawn rate with level
	this->spawnTimermax = std::max(20.f, 50.f - (level * 2.f)); // Adjust spawn rate as level increases

	if (this->spawnTimer >= this->spawnTimermax)
	{
		float playerX = this->player->getPos().x;
		float randomX;
		bool validPosition = false;
		int attempts = 0; // Track attempts
		const int maxAttempts = 100; // Maximum attempts to find a valid position

		while (!validPosition && attempts < maxAttempts)
		{
			randomX = playerX + static_cast<float>(rand() % 401 - 200);
			randomX = std::max(0.f, std::min(randomX, static_cast<float>(this->window->getSize().x)));

			validPosition = true;

			for (const auto& enemy : this->enemies)
			{
				if (std::abs(enemy->getPos().x - randomX) < 50.f) // Minimum distance of 50 units
				{
					validPosition = false;
					break;
				}
			}

			++attempts;
		}

		if (validPosition) // Only spawn if a valid position was found
		{
			this->enemies.push_back(new Enemy(randomX, -100.f));
			this->spawnTimer = 0.f;
		}
	}

	for (size_t i = 0; i < this->enemies.size(); ++i) {
		this->enemies[i]->update(this->level);
		this->enemies[i]->updateattack(this->getlevel());
		// Remove enemy if out of bounds
		if (this->enemies[i]->getBounds().top > this->window->getSize().y) {
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);
			--i;
			continue;
		}

		if (this->enemies[i]->canShoot())
		{
			this->enemyBullets.push_back(new Bullet(this->textures["BULLET"],this->enemies[i]->getPos().x + 20, this->enemies[i]->getBounds().top + this->enemies[i]->getBounds().height, 0.f, 1.f, 3.f * level, 2 * level, this->level));  // Downward bullet		

		}

	}
}


void Game::updateEnemyBullets()
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
void Game::updateEnemiesCombat()
{
	for (size_t i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_removed = false;

		// Check collision with bullets
		for (size_t k = 0; k < this->bullets.size() && !enemy_removed; ++k)
		{
			if (this->bullets[k]->getBounds().intersects(this->enemies[i]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();
				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);
				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);
				enemy_removed = true;
			}
		}

		// Check collision with player if enemy is still present
		if (!enemy_removed && this->enemies[i]->getBounds().intersects(this->player->getBounds()))
		{
			// Handle player hit (e.g., decrease health)

			this->player->losehp(enemies[i]->getDamage());
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);

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
		star->update(levelSpeedMultiplier);
	}
}

void Game::updateUI() {
	float playerHealthPercent = (this->player->getHp() / this->player->getHpMax());


	this->ui->updateHealthBar(playerHealthPercent, this->player->getPos().x + 10, this->player->getPos().y + 95);
	int score = this->getpoints();
	if (score > 20 * this->getlevel())
	{
		this->updatelevel();
	}
	int level = this->getlevel();
	this->ui->updateScoreAndLevel(score, level);
	if (this->player->getHp() == 0)
	{
		this->ui->endgame();

	}

}


void Game::update()
{
	
	this->updatePollEvents();
	if (this->player->getHp() != 0)
	{

		this->updateInput();
		this->updateStars();
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

void Game::render()
{
	this->window->clear(sf::Color::Black);

	for (auto* star : this->stars)
		star->render(*this->window);

	this->player->render(*this->window);

	for (auto* bullet : this->bullets)
		bullet->render(this->window);

	for (auto* enemy : this->enemies)
		enemy->render(this->window);

	for (auto* bullet : this->enemyBullets) {
		bullet->render(this->window);
	}
	this->renderUI();
	this->window->display();
}

void Game::resetgame()
{
	this->player->resetstats();
	this->player->move(this->window->getSize().x / 2.f, this->window->getSize().y - 50.f, *window);
	this->level = 1;

}