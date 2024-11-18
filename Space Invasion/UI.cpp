#include "UI.h"
#include "Game.h"
#include "font1.h"
UI::UI(sf::RenderWindow& win) : window(win) {
    // Initialize the health bar
    this->initializeHealthBar();
    this->initGUI();

}

void UI::initializeHealthBar() {
    // Background bar
    this->hpback.setSize(sf::Vector2f(55.f, 3.f)); // Set size for background


    // Health bar (it will be dynamically resized later)
    this->hpbar.setSize(sf::Vector2f(55.f, 3.f)); // Initial size, can be updated later



}


void UI::updateHealthBar(float healthPercent, float posx, float posy) {
    // Calculate the width based on health percentage
    float maxBarWidth = 55.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    this->hpback.setPosition(posx, posy); // Set position for the background bar
    this->hpback.setFillColor(sf::Color(25, 25, 25));

    this->hpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->hpbar.setPosition(posx, posy); // Set position for the background bar
    // Change color based on health percentage
    if (healthPercent > 0.5f) {
        this->hpbar.setFillColor(sf::Color::Green); // Green for healthy
    }
    else {
        this->hpbar.setFillColor(sf::Color::Red); // Red for low health
    }


}

void UI::updateScoreAndLevel(int score, int level)
{
    this->ptext.setPosition(690, 25);
    this->ptext.setString("Score:" + std::to_string(score) + "\nLevel: " + std::to_string(level));
}

void UI::endgame()
{
    this->GameOver.setString("Game Over");
    this->GameOver.setPosition(
        this->window.getSize().x / 2.f - this->GameOver.getGlobalBounds().width / 2.f,
        this->window.getSize().y / 2.f - this->GameOver.getGlobalBounds().height / 2.f);

}

void UI::initGUI() {
    if (!this->font.loadFromMemory(font1_ttf,font1_ttf_len)) {
        std::cout << "Error: Font not loaded\n";
    }

    this->ptext.setFont(this->font);
    this->ptext.setCharacterSize(25);
    this->ptext.setFillColor(sf::Color::Red);

    this->GameOver.setFont(this->font);
    this->GameOver.setCharacterSize(60);
    this->GameOver.setFillColor(sf::Color::Red);


}

void UI::render(sf::RenderTarget* target) {
    // Draw background bar first
    target->draw(this->hpback);

    // Draw the health bar on top of the background
    target->draw(this->hpbar);

    // Optionally, draw the score and level text
    target->draw(this->ptext);

    target->draw(this->GameOver);
}
