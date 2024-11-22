#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
class UI {
public:    //Gamestate
   
    sf::RenderWindow& window;
    sf::Clock clock;
    int finalscore;

    //for game window
    sf::Text title;
    sf::Text play_button;
    sf::RectangleShape play_button_outline;
    sf::Text instructions_button;
    sf::RectangleShape instructions_button_outline;
    sf::Text scores_button;
    sf::RectangleShape scores_button_outline;

    // back button
    sf::Text score;
    sf::Text back_button;
    sf::RectangleShape back_button_outline;
    sf::Text Replay;
    sf::RectangleShape Replay_outline;
    sf::Text Close;
    sf::RectangleShape Close_outline;

    // variables for instructions elements
    sf::Text how_to_play_title;
    sf::Text objectives;

    // functions for menu
    void update();
    void set_menu_objects(int width,int height);
    void load_menu();


    // functions for how to play
    void set_instructions_objects(int width,int height);
    void load_instructions();

    
    void set_scores(int x);
    void load_score();
    void load_end_page();
    void load_game();

    // GUI components
   
    sf::Font font;
    sf::Text ptext;
    sf::Text GameOver;
    sf::RectangleShape hpbar;
    sf::RectangleShape hpback;
    sf::RectangleShape bosshpbar;
    sf::RectangleShape bosshpback;


    sf::Sprite hearts[3];
    sf::Texture heartTexture;
    sf::RectangleShape livesContainer;
    sf::RectangleShape scoreLevelContainer;
    sf::Texture exptext;
    sf::Sprite explosion;

public:
    // Constructor initializes window reference
    explicit UI(sf::RenderWindow& win);

    // Function to initialize health bar (instead of calling it HealthBar)
    void initializeHealthBar();

    // Function to update health bar based on current health percentage
    void updateHealthBar(float healthPercent, float posx, float posy, sf::Color color);
    void updateHearts(int lives,float healthPercent);
    void updatebossHealthBar(float healthPercent, float posx, float posy, sf::Color color);
    void updateexplosion(float posx, float posy);

    void updateScoreAndLevel(int score, int level,float x, float y);
    void endgame();
    // Function to initialize text and fonts
    void initGUI();

    // Function to render UI components
    void render(sf::RenderTarget* target);
};

#endif // UI_HPP
