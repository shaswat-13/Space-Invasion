#include "UI.h"
#include "Game.h"
#include "cosmicalien.h"
#include "cmath"
#include "spritesheet.h"
UI::UI(sf::RenderWindow& win) : window(win) {
    // Initialize the health bar
    this->initializeHealthBar();
    this->initGUI();
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    this->set_menu_objects(desktopMode.width, desktopMode.height);
    this->set_instructions_objects(desktopMode.width, desktopMode.height);
   

}

void UI::initializeHealthBar() {
    // Background bar
    this->hpback.setSize(sf::Vector2f(55.f, 3.f)); // Set size for background


    // Health bar (it will be dynamically resized later)
    this->hpbar.setSize(sf::Vector2f(55.f, 3.f)); // Initial size, can be updated later



}


void UI::updateHealthBar(float healthPercent, float posx, float posy, sf::Color color)
{
    // Calculate the width based on health percentage
    float maxBarWidth = 55.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    
    this->hpback.setPosition(posx-2, posy+3); // Set position for the background bar
    this->hpback.setFillColor(sf::Color(25, 25, 25));
    this->hpback.setOutlineThickness(1.f);
    this->hpback.setOutlineColor(sf::Color::White);

    this->hpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->hpbar.setPosition(posx-2, posy+3); // Set position for the background bar
    this->hpbar.setOutlineThickness(1.f);
    this->hpbar.setOutlineColor(sf::Color::Red);
    

    this->heart.setPosition(posx -25, posy-8);
    this->heart.setScale(0.03, 0.03);
    int frameIndex = 0;
    if (healthPercent > 0.8f) {
        frameIndex = 0; // Full heart
    }
    else if (healthPercent > 0.6f) {
        frameIndex = 1; // 80% heart
    }
    else if (healthPercent > 0.4f) {
        frameIndex = 2; // 60% heart
    }
    else if (healthPercent > 0.1f) {
        frameIndex = 3; // 40% heart
    }
    else  {
        frameIndex = 4; // Empty heart
    }

    const int frameWidth = 669; // Width of each frame
    const int frameheight = 603;
    

    this->heart.setTextureRect(sf::IntRect(frameWidth * frameIndex , 0, frameWidth, frameheight));

    // Change color based on health percentage
    if (healthPercent > 0.4f) {
        this->hpbar.setFillColor(color); // Green for healthy

    }
    else {
        int alpha = std::sin(clock.getElapsedTime().asSeconds() * 10) * 127 + 128; // Pulse effect
        this->hpbar.setFillColor(sf::Color(255, 0, 0, alpha)); // Pulsing red
    }

}
void UI::updateHeart(float healthPercent)
{

}
void UI::updatebossHealthBar(float healthPercent, float posx, float posy, sf::Color color) {
    // Calculate the width based on health percentage
    float maxBarWidth = 70.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    this->bosshpback.setPosition(posx, posy); // Set position for the background bar
    this->bosshpback.setFillColor(sf::Color(25, 25, 25));

    this->bosshpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->bosshpbar.setPosition(posx, posy); // Set position for the background bar
    // Change color based on health percentage
    if (healthPercent > 0.5f) {
        this->bosshpbar.setFillColor(color); // Green for healthy
    }
    else {
        this->bosshpbar.setFillColor(sf::Color::Red); // Red for low health
    }


}

void UI::updateexplosion(float posx, float posy)
{

}




void UI::updateScoreAndLevel(int score, int level,float x, float y)
{
    this->ptext.setPosition(690, 25);
    this->ptext.setString("Score:" + std::to_string(score) + "\nLevel: " + std::to_string(level));
    this->ptext.setPosition(x, y);

}

void UI::endgame()
{
    this->GameOver.setString("Game Over");
    this->GameOver.setPosition(
        this->window.getSize().x / 2.f - this->GameOver.getGlobalBounds().width / 2.f,
        this->window.getSize().y / 2.f - this->GameOver.getGlobalBounds().height / 2.f);

}

void UI::initGUI() {
    if (!this->font.loadFromMemory(CosmicAlien_ttf,CosmicAlien_ttf_len)) {
        std::cout << "Error: Font not loaded\n";
    }
    if (!this->heartTexture.loadFromMemory(spritesheet_png, spritesheet_png_len)) {
        std::cout << "Error: Font not loaded\n";
    }
    else 
    {
        std::cout << "Texture loaded successfully. Size: "
            << this->heartTexture.getSize().x << "x"
            << this->heartTexture.getSize().y << std::endl;
    }
    this->heart.setTexture(heartTexture);
    
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
    target->draw(this->heart);

    target->draw(this->bosshpback);
    target->draw(this->bosshpbar);


    // Optionally, draw the score and level text
    target->draw(this->ptext);

    target->draw(this->GameOver);
}

void UI::update()
{
    sf::Vector2u windowSize = window.getSize(); // Get the current window size
    this->set_menu_objects(windowSize.x, windowSize.y);
    this->set_instructions_objects(windowSize.x, windowSize.y);

}


void UI::set_menu_objects(int width, int height)
{
    // Dynamic font sizes based on window height
    

    // Title
    title.setString("SPACE INVASION");
    title.setFont(font);
    title.setFillColor(sf::Color::White);
    float baseHeight = 800.0f; // Reference height
    int titleSize = static_cast<int>(height * 50.0f / baseHeight); // Title character size
    int buttonSize = static_cast<int>(height * 30.0f / baseHeight);
    title.setCharacterSize(titleSize);

    sf::FloatRect titleBounds = title.getLocalBounds();
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    title.setPosition(
        centerX - titleBounds.width / 2.0f,
        centerY - 200 // Offset upwards
    );
    // Play Button (Center)
    play_button.setString("PLAY");
    play_button.setFont(font);
    play_button.setCharacterSize(buttonSize);
    play_button.setFillColor(sf::Color::White);
    play_button.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f, height * 0.5f);  // Centered both horizontally and vertically

    // Play Button Outline (Center)
    play_button_outline.setSize(sf::Vector2f(play_button.getGlobalBounds().width + 50, play_button.getGlobalBounds().height + 40));
    play_button_outline.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f - 25, height * 0.5f + 7);
    play_button_outline.setOutlineColor(sf::Color::White);
    play_button_outline.setFillColor(sf::Color::Black);
    play_button_outline.setOutlineThickness(3.0f);

    // Instructions Button (Bottom-left corner)
    instructions_button.setString("HOW TO PLAY");
    instructions_button.setFont(font);
    instructions_button.setCharacterSize(buttonSize);
    instructions_button.setFillColor(sf::Color::White);
    instructions_button.setPosition(0.05f * width, height * 0.9f);  // 5% from the left, 90% from the top

    instructions_button_outline.setPosition(0.05f * width - 20, height * 0.9f); // Outline adjustment to stay with the button
    instructions_button_outline.setSize(sf::Vector2f(instructions_button.getGlobalBounds().width + 40, instructions_button.getGlobalBounds().height + 30));
    instructions_button_outline.setOutlineColor(sf::Color::White);
    instructions_button_outline.setFillColor(sf::Color::Black);
    instructions_button_outline.setOutlineThickness(3.0f);

    // Scores Button (Bottom-right corner)
    scores_button.setString("SCORES");
    scores_button.setFont(font);
    scores_button.setCharacterSize(buttonSize);
    scores_button.setFillColor(sf::Color::White);
    scores_button.setPosition(0.95f * width - scores_button.getGlobalBounds().width, height * 0.9f);  // 95% from the right, 90% from the top

    scores_button_outline.setPosition(0.95f * width - scores_button.getGlobalBounds().width - 20, height * 0.9f); // Outline adjustment to stay with the button
    scores_button_outline.setSize(sf::Vector2f(scores_button.getGlobalBounds().width + 40, scores_button.getGlobalBounds().height + 30));
    scores_button_outline.setOutlineColor(sf::Color::White);
    scores_button_outline.setFillColor(sf::Color::Black);
    scores_button_outline.setOutlineThickness(3.0f);
}




void UI::load_menu()
{
    window.draw(title);
    window.draw(play_button_outline);
    window.draw(play_button);
    window.draw(instructions_button_outline);
    window.draw(instructions_button);
    window.draw(scores_button_outline);
    window.draw(scores_button);
}

void UI::set_instructions_objects(int width,int height)
{
    // title
    how_to_play_title.setString("HOW TO PLAY?");
    how_to_play_title.setFont(font);
    how_to_play_title.setFillColor(sf::Color::White);
    how_to_play_title.setCharacterSize(72);
    how_to_play_title.setPosition(width / 2 - how_to_play_title.getGlobalBounds().width / 2, 0.1 * height);

    // first point
    objectives.setString("1. Press Arrow Keys to Move\n2. Press Space to Shoot\n3. Shoot Enemies to Destroy Them\n4. Don't let Enemy reach the bottom\n5. Avoid Enemy Bullets\n6. Get High Score");
    objectives.setFont(font);
    objectives.setFillColor(sf::Color::White);
    objectives.setCharacterSize(32);
    objectives.setPosition(0.15 * width, 300);

    // back button
    back_button.setString("BACK");
    back_button.setFont(font);
    back_button.setFillColor(sf::Color::White);
    back_button.setCharacterSize(30);
    back_button.setPosition(0.05 * width, 0.05 * height);

    back_button_outline.setFillColor(sf::Color::Black);
    back_button_outline.setOutlineColor(sf::Color::White);
    back_button_outline.setOutlineThickness(2.0f);
    back_button_outline.setSize(sf::Vector2f(back_button.getGlobalBounds().width + 20, back_button.getGlobalBounds().height + 20));
    back_button_outline.setPosition(0.05 * width - 10, 0.05 * height);
}

void UI::load_instructions()
{
    window.draw(how_to_play_title);
    window.draw(objectives);
    window.draw(back_button_outline);
    window.draw(back_button);
}

void UI::set_scores()
{
}

void UI::load_scores()
{
}

void UI::load_end_page()
{
}

void UI::load_game()
{
}
