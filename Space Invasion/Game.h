#pragma once
#include<map>
#include<string>
#include<sstream>
#include"Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include "Star.h"
#include "UI.h"
class Game
{
private:
	enum class GameState
	{
		MENU,
		SCORES,
		INSTRUCTIONS,
		GAME,
		END,
		BACK,
		GAME_OVER,
		Replay

	};
	//window
	GameState game_state;
	int level = 5;
	int lives = 3;
	sf::RenderWindow* window;
	bool isPulsating;

	//resources
	sf::Clock clock;
	sf::Clock eventclock;
	sf::Font font2;
	sf::Text frameRateText;
	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;
	std::vector<Star*> stars;
	std::vector<Bullet*> enemyBullets;

	// starting screen
	bool isStartingScreenActive;
	sf::Text startTitle;
	sf::Text startInstruction;
	void initStartingScreen();

	//score
	void loadHighScore();
	void saveHighScore();

	//Player
	Player* player;
	UI* ui;

	//game events
	void handleMenuEvents(sf::Event &event);
	void handleInstructionsEvents(sf::Event& event);
	void handleGameEvents(sf::Event& event);
	void handleEndEvents(sf::Event& event);
	void handleGameOverEvents(sf::Event& event);




	//systems
	unsigned points;

	//Enemies

	float spawnTimer;
	float spawnTimermax;
	std::vector<Enemy*>enemies;
	Enemy* enemy;
	std::vector<Enemy*>boss;
	
	bool bossSpawned=false;


	//private functions
	void initwindow();
	void initPlayer();
	void initStars();
	void initTextures();
	void initSystems();
	void initEnemies();
	void initUI();

	void updateEnemyBullets();

public:
	Game();
	virtual ~Game();
	// functions
	void run();
	int getpoints();
	int getlevel();
	void updatelevel();
	int getlives();
	void updatelives();

	void updatePollEvents();
	void handleWindowResize(unsigned int,unsigned int);
	void handleResize();

	void updateeffects();
	void updateInput();
	void updateEnemiesCombat();
	void updateEnemies();
	void updateBullets();
	void updateStars();
	void updateUI();
	void update();

	void updateStartingScreen(sf::RenderWindow& window);
	// New function for handling starting screen updates
	void renderStartingScreen(); // New function for rendering the starting screen

	void renderUI();
	void render();
	void resetgame();


};