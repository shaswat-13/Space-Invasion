#pragma once
#include <map>
#include <string>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
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
        Name_Input,
        END,
        BACK,
        GAME_OVER,
        Replay
    };

    // Window
    GameState game_state;
    int level = 5;
    int lives = 3;
    sf::RenderWindow* window;
    bool isPulsating;

    // Resources
    sf::Clock clock;
    sf::Clock eventclock;
    sf::Font font2;
    sf::Text frameRateText;
    std::map<std::string, sf::Texture*> textures;
    std::vector<Bullet*> bullets;
    std::vector<Star*> stars;
    std::vector<Bullet*> enemyBullets;

    // Starting Screen
    bool isStartingScreenActive;
    sf::Text startTitle;
    sf::Text startInstruction;
    void initStartingScreen();

    // Score
    int highscore;
    sf::Text highScoreText;
    std::string playerName;  // Store player's name
    void loadHighScore();
    void saveScore();  // Save score with name

    // Player
    Player* player;
    UI* ui;

    // Game Events
    void handleMenuEvents(sf::Event& event);
    void handleInstructionsEvents(sf::Event& event);
    void handleGameEvents(sf::Event& event);
    void handleEndEvents(sf::Event& event);
    void handleGameOverEvents(sf::Event& event);
    void handlescoreevents(sf::Event& event);

    // Systems
    unsigned points;

    // Enemies
    float spawnTimer;
    float spawnTimermax;
    std::vector<Enemy*> enemies;
    Enemy* enemy;
    std::vector<Enemy*> boss;

    bool bossSpawned = false;

    // Private Functions
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
    // Functions
    void run();
    int getpoints();
    int getlevel();
    void updatelevel();
    int getlives();
    void updatelives();

    void updatePollEvents();
    void handleWindowResize(unsigned int, unsigned int);
    void handleResize();

    void updateInput();
    void updateEnemiesCombat();
    void updateEnemies();
    void updateBullets();
    void updateStars();
    void updateUI();
    void update();

    void updateStartingScreen(sf::RenderWindow& window);
    void renderStartingScreen();  // New function for rendering the starting screen
    void displayHighScore();  // Display high score with player name

    void renderUI();
    void render();
    void resetgame();

    // New functions for handling user input and displaying the name
    void getPlayerNameInput();
};
