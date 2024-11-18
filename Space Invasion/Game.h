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
	//window
	int level = 1;
	sf::RenderWindow* window;


	//resources
	sf::Clock clock;

	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;
	std::vector<Star*> stars;
	std::vector<Bullet*> enemyBullets;
	//Player
	Player* player;
	UI* ui;




	//systems
	unsigned points;

	//Enemies

	float spawnTimer;
	float spawnTimermax;
	std::vector<Enemy*>enemies;
	Enemy* enemy;


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
	void updatePollEvents();
	void updateInput();
	void updateEnemiesCombat();
	void updateEnemies();
	void updateBullets();
	void updateStars();
	void updateUI();
	void update();
	void renderUI();
	void render();
	void resetgame();


};