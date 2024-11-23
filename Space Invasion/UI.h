#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>

class UI {
    
public:    //Gamestate
   
    sf::RenderWindow& window;
    sf::Clock clock;
    int finalscore;
     bool ButtonClicked;

    //game sounds
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;



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
    sf::Text Restart;
    sf::Text pause;
    sf::Text Instructions;
    sf::Text resume;
    sf::Text exit;

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
    void pause_page();

    // GUI components
   
    sf::Font font;
    sf::Text ptext;
    sf::Text GameOver;
    sf::RectangleShape hpbar;
    sf::RectangleShape hpback;
    sf::RectangleShape bosshpbar;
    sf::RectangleShape bosshpback;

    sf::Clock animationClock; // To control frame timing
    std::vector<sf::Sprite> explosions;  // Vector to store active explosions
    int currentFrame = 0;  // Track the current frame of the animation
    const int totalFrames = 10;  // Total frames in the explosion sprite sheet
    sf::Sprite hearts[3];
    sf::Texture heartTexture;
    sf::RectangleShape livesContainer;
    sf::RectangleShape scoreLevelContainer;
    sf::Texture exptext;
    sf::Sprite exp;

    sf::Text music_button;
    sf::RectangleShape music_button_outline;

    sf::Music backgroundMusic;   // To hold background music
    bool isMusicOn = true;			  // To keep track of music state

public:
    // Constructor initializes window reference
    explicit UI(sf::RenderWindow& win);

    // Function to initialize health bar (instead of calling it HealthBar)
    void initializeHealthBar();

    // Function to update health bar based on current health percentage
    void updateHealthBar(float healthPercent, float posx, float posy, sf::Color color);
    void updateHearts(int lives,float healthPercent);
    void updatebossHealthBar(float healthPercent, float posx, float posy, sf::Color color);
    void updateExplosion(float posx, float posy);
    void updateAllExplosions(sf::RenderWindow& window);


    void updateScoreAndLevel(int score, int level,float x, float y);
    void endgame();
    // Function to initialize text and fonts
    void initGUI();

    // Function to render UI components
    void render(sf::RenderTarget* target);
    void playClickSound();
    void initializeSounds();


    sf::Text credits_button;
    sf::RectangleShape credits_button_outline;
    sf::Text credits_title;
    sf::Text credits_text;


    void set_credits_objects(int width, int height);
    void load_credits();

};

#endif // UI_HPP
