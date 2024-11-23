#pragma once
#include <map>
#include <string>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Star.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class Game
{
private:
    enum class GameState
    {
        MENU,
        SCORES,
        INSTRUCTIONS,
        GAME,
        Pause,
        Name_Input,
        END,
        BACK,
        GAME_OVER,
        Replay,
        Credits
    };

    // Window
    GameState game_state;
    GameState prev_state;
    int level = 1;
    int lives = 4;
    int maxEnemies; // Dynamic maximum number of enemies

    sf::RenderWindow* window;
    bool isPulsating;

    // Resources//sound
    sf::SoundBuffer killSoundBuffer;
    sf::Sound killSound;
    sf::SoundBuffer pausesoundbuffer;
    sf::Sound pausesound;

    sf::Clock clock;
    sf::Clock eventclock;
    sf::Font font2;
    sf::Text frameRateText;

    int bossSpawnedCount;

    std::map<std::string, sf::Texture*> textures;
    std::vector<Bullet*> bullets;
    std::vector<Star*> stars;
    std::vector<Bullet*> enemyBullets;
    std::vector<Bullet*> bossbullets;
    std::vector<UI*>bosshp;

    // Starting Screen
    bool isStartingScreenActive;
    sf::Text startTitle;
    sf::Text startInstruction;

    // Score
    int highscore;
    sf::Text highScoreText;
    sf::Text scores;
    sf::String playerName;  // Store player's name
    void saveScore();  // Save score with name
    bool scoreSaved = false;


    // Player
    Player* player;
    UI* ui;

    // Game Events
    void handleMenuEvents(sf::Event& event);
    void handleInstructionsEvents(sf::Event& event);
    void handleGameEvents(sf::Event& event);
    void handlePauseevents(sf::Event& event);
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
    std::vector<float> occupiedPositions; // Track occupied X positions

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

    void displayHighScore();  // Display high score with player name

    void renderUI();
    void render();
    void resetgame();

    // New functions for handling user input and displaying the name
    void getPlayerNameInput();
};
