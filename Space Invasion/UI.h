#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class UI {
private:
    sf::RenderWindow& window;

    // GUI components
    sf::Font font;
    sf::Text ptext;
    sf::Text GameOver;
    sf::RectangleShape hpbar;
    sf::RectangleShape hpback;

public:
    // Constructor initializes window reference
    explicit UI(sf::RenderWindow& win);

    // Function to initialize health bar (instead of calling it HealthBar)
    void initializeHealthBar();

    // Function to update health bar based on current health percentage
    void updateHealthBar(float healthPercent, float posx, float posy);
    void updateScoreAndLevel(int score, int level);
    void endgame();
    // Function to initialize text and fonts
    void initGUI();

    // Function to render UI components
    void render(sf::RenderTarget* target);
};

#endif // UI_HPP
