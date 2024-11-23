#include "UI.h"
#include "Game.h"
#include "cosmicalien.h"
#include "cmath"
#include "spritesheet.h"
#include "exp.h"
UI::UI(sf::RenderWindow& win) : window(win) {
    // Initialize the health bar
    this->initializeHealthBar();
    this->initializeSounds();
    this->initGUI();
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    this->set_menu_objects(desktopMode.width, desktopMode.height);
    this->set_instructions_objects(desktopMode.width, desktopMode.height);
    if (!backgroundMusic.openFromFile("Sound/backmusic.wav"))
    {
        std::cout << "Error: Could not load background music" << std::endl;
    }
    else {
        backgroundMusic.setVolume(50);
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

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
    float maxBarWidth = 68.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    
    this->hpback.setPosition(posx-2, posy+3); // Set position for the background bar
    this->hpback.setFillColor(sf::Color(25, 25, 25));
    this->hpback.setOutlineThickness(1.f);
    this->hpback.setOutlineColor(sf::Color::Black);

    this->hpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->hpbar.setPosition(posx-2, posy+3); // Set position for the background bar
    this->hpbar.setOutlineThickness(1.f);
    this->hpbar.setOutlineColor(sf::Color::Black);
    

    // Change color based on health percentage
    if (healthPercent > 0.4f) {
        this->hpbar.setFillColor(color); // Green for healthy

    }
    else {
        int alpha = std::sin(clock.getElapsedTime().asSeconds() * 10) * 127 + 128; // Pulse effect
        this->hpbar.setFillColor(sf::Color(255, 0, 0, alpha)); // Pulsing red
    }

}
void UI::updateHearts(int lives, float healthPercent) {
    for (int i = 0; i < 3; ++i) {
        int frameIndex = 4; // Default to empty heart

        if (i < lives) { // Only update hearts for remaining lives
            if (i == lives - 1) {
                // Update the heart corresponding to the last remaining life
                if (healthPercent > 0.8f) frameIndex = 0; // Full heart
                else if (healthPercent > 0.6f) frameIndex = 1; // 80% heart
                else if (healthPercent > 0.4f) frameIndex = 2; // 60% heart
                else if (healthPercent > 0.1f) frameIndex = 3; // 40% heart
            }
            else {
                // Hearts not corresponding to the last remaining life stay full
                frameIndex = 0;
            }
        }

        const int frameWidth = 669;
        const int frameHeight = 603;

        // Map hearts array index (left-to-right)
        hearts[i].setTextureRect(sf::IntRect(frameWidth * frameIndex, 0, frameWidth, frameHeight));
    }
}


void UI::updatebossHealthBar(float healthPercent, float posx, float posy, sf::Color color) {
    // Calculate the width based on health percentage
    float maxBarWidth = 68.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    this->bosshpback.setPosition(posx, posy); // Set position for the background bar
    this->bosshpback.setFillColor(sf::Color(25, 25, 25));

    this->bosshpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->bosshpbar.setPosition(posx, posy); // Set position for the background bar
    this->bosshpbar.setOutlineColor(sf::Color::Black);

    // Change color based on health percentage
    
        this->bosshpbar.setFillColor(color); // Green for healthy
    

}


// This function will update the explosion at the given position.
void UI::updateExplosion(float posx, float posy)
{
    const float framewidth = 644.f;
    const float frameheight = 644.f;

    // Ensure the explosions vector is not empty before accessing the last element
    if (!explosions.empty()) {
        sf::Sprite& exp = explosions.back(); // Get the last added explosion sprite
        exp.setPosition(posx, posy); // Set the position of the last explosion sprite

        if (animationClock.getElapsedTime().asMilliseconds() > 100) { // Change frame every 100ms
            currentFrame++;

            if (currentFrame >= totalFrames) {
                currentFrame = totalFrames - 1; // Stop at the last frame
            }

            // Update the texture rect to show the next frame
            exp.setTextureRect(sf::IntRect(currentFrame * framewidth, 0, framewidth, frameheight));

            animationClock.restart();
        }

    }
    else {
        // Handle case where explosions vector is empty
        std::cout << "No explosions to update." << std::endl;
    }
    explosions.push_back(exp);

}

// This function will update all explosions, calling `updateExplosion` for each.
void UI::updateAllExplosions(sf::RenderWindow& window)
{
    for (auto it = explosions.begin(); it != explosions.end();) {
        updateExplosion(it->getPosition().x, it->getPosition().y);  // Update each explosion

        window.draw(*it);  // Render each explosion

        // If the explosion has finished, remove it from the list
        if (currentFrame == totalFrames - 1) {
            it = explosions.erase(it);  // Remove the explosion from the list
        }
        else {
            ++it;  // Continue to the next explosion
        }
    }
}





void UI::updateScoreAndLevel(int score, int level,float x, float y)
{

    this->ptext.setString("Score:" + std::to_string(score) + "\nLevel:" + std::to_string(level));
    this->ptext.setCharacterSize(30);
    this->ptext.setPosition(x, y);
    this->finalscore = score;
}


void UI::endgame()
{
    window.draw(GameOver);
    window.draw(score);
    window.draw(Replay);
    window.draw(Close);
    window.draw(back_button_outline);

    window.draw(back_button);


}

void UI::initGUI() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

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
    for (int i = 0; i < 3; ++i) {
        hearts[i].setTexture(heartTexture);
        hearts[i].setScale(0.07f, 0.07f); // Adjust size (10% of original texture size)
        hearts[i].setPosition(0.05*desktopMode.width + i * 50.f, desktopMode.height*0.05); // Increase spacing between hearts

        // Full heart frame initially
        hearts[i].setTextureRect(sf::IntRect(0, 0, 669, 603));
        hearts[i].setColor(sf::Color(255, 50,50, 255)); // Fully opaque red

    }
    
   


    this->ptext.setFont(this->font);
    this->ptext.setCharacterSize(25);
    this->ptext.setFillColor(sf::Color::Red);

   


}

void UI::render(sf::RenderTarget* target) {
    
    
    // Draw background bar first
    target->draw(this->hpback);

    // Draw the health bar on top of the background
    target->draw(this->hpbar);
    for (int i = 0; i < 3; ++i) {
        target->draw(hearts[i]);
    }

    target->draw(this->bosshpback);
    target->draw(this->bosshpbar);

    
    // Optionally, draw the score and level text
    target->draw(this->ptext);

    
}

void UI::update() {
    sf::Color normalColor(255, 255, 255);  // White
    sf::Color hoverColor(255, 223, 0);    // Yellowish
    sf::Color clickColor(255, 180, 0);    // Darker orange

    // Store scale factors
    sf::Vector2f normalScale(1.0f, 1.0f);
    sf::Vector2f hoverScale(1.1f, 1.1f);
    sf::Vector2f clickScale(1.05f, 1.05f);
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    static bool musicButtonPressed = false;

    // Play button
    if (play_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        play_button_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Bright yellow outline
        play_button.setFillColor(sf::Color(100, 255, 255)); // Light green button
        play_button.setScale(hoverScale);
        play_button_outline.setScale(hoverScale); // Slightly larger scale

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ButtonClicked) {
            // Click state
            play_button_outline.setFillColor(clickColor); // Vibrant orange outline
            play_button.setFillColor(sf::Color(80, 200, 80)); // Darker green button
            play_button.setScale(clickScale);
            play_button_outline.setScale(clickScale); // Slightly larger scale

            ButtonClicked = true;  // Register the click
        }
    }
    else {
        // Normal state
        play_button_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        play_button.setFillColor(sf::Color::White); // White button
        play_button.setScale(normalScale);
        play_button_outline.setScale(normalScale);

        // Only reset ButtonClicked if the mouse isn't pressed anymore
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            ButtonClicked = false;
        }
    }

    if (instructions_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        instructions_button_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        instructions_button.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        instructions_button.setScale(1.1f, 1.1f); // Slightly larger scale for button
        instructions_button_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ButtonClicked) {
            // Click state
            instructions_button.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            instructions_button_outline.setScale(1.05f, 1.05f); // Slightly smaller scale for outline

            ButtonClicked = true;  // Register the click
        }
    }
    else {
        // Normal state
        instructions_button_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        instructions_button.setFillColor(sf::Color::White); // White button
        instructions_button.setScale(1.0f, 1.0f); // Default scale for button
        instructions_button_outline.setScale(1.0f, 1.0f); // Default scale for outline

        // Only reset ButtonClicked if the mouse isn't pressed anymore
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            ButtonClicked = false;
        }
    }


    // Scores button
    if (scores_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        scores_button_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        scores_button.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        scores_button.setScale(1.1f, 1.1f); // Slightly larger scale for button
        scores_button_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ButtonClicked) {
            // Click state
            scores_button.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            scores_button_outline.setScale(1.05f, 1.05f); // Slightly smaller scale for outline

            ButtonClicked = true;  // Register the click
        }
    }
    else {
        // Normal state
        scores_button_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        scores_button.setFillColor(sf::Color::White); // White button
        scores_button.setScale(1.0f, 1.0f); // Default scale for button
        scores_button_outline.setScale(1.0f, 1.0f); // Default scale for outline

        // Only reset ButtonClicked if the mouse isn't pressed anymore
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            ButtonClicked = false;
        }
    }


    // Music button with improved toggle logic
    if (music_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        music_button_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        music_button.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        music_button.setScale(1.1f, 1.1f); // Slightly larger scale for button
        music_button_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!musicButtonPressed) {  // Only trigger once when button is first pressed
                musicButtonPressed = true;
                playClickSound();
                if (isMusicOn) {
                    backgroundMusic.stop();
                    music_button.setString("MUSIC:OFF");
                    music_button.setFillColor(sf::Color::Red);
                    music_button_outline.setSize(sf::Vector2f(music_button.getGlobalBounds().width, music_button.getGlobalBounds().height + 20));

                }
                else {
                    backgroundMusic.play();
                    music_button.setString("MUSIC:ON");
                    music_button.setFillColor(sf::Color::White);
                    music_button_outline.setSize(sf::Vector2f(music_button.getGlobalBounds().width , music_button.getGlobalBounds().height + 20));

                }
                isMusicOn = !isMusicOn;
            }
        }
        else {
            musicButtonPressed = false;  // Reset when button is released
        }
    }
    else {
        // Normal state
        music_button_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        music_button.setScale(1.0f, 1.0f); // Default scale for button
        music_button_outline.setScale(1.0f, 1.0f); // Default scale for outline
    }


    // Back button
    if (back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        back_button_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        back_button.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        back_button.setScale(1.1f, 1.1f); // Slightly larger scale for button
        back_button_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            back_button.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            back_button_outline.setScale(1.05f, 1.05f); // Slightly smaller scale for outline

        }
    }
    else {
        // Normal state
        back_button_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        back_button.setFillColor(sf::Color::White); // White button
        back_button.setScale(1.0f, 1.0f); // Default scale for button
        back_button_outline.setScale(1.0f, 1.0f); // Default scale for outline
    }

    // Replay button
    if (Replay_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        Replay_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        Replay.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        Replay.setScale(1.1f, 1.1f); // Slightly larger scale for button
        Replay_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Replay.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            Replay_outline.setScale(1.05f, 1.05f); // Slightly smaller scale for outline
        }
    }
    else {
        // Normal state
        Replay_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        Replay.setFillColor(sf::Color::White); // White button
        Replay.setScale(1.0f, 1.0f); // Default scale for button
        Replay_outline.setScale(1.0f, 1.0f); // Default scale for outline
    }

    // Close button
    if (Close_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        Close_outline.setFillColor(sf::Color(0, 255, 255, 100)); // Light cyan outline
        Close.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        Close.setScale(1.1f, 1.1f); // Slightly larger scale for button
        Close_outline.setScale(1.1f, 1.1f); // Slightly larger scale for outline

        // Check if the mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Close.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            Close_outline.setScale(1.05f, 1.05f); // Slightly smaller scale for outline
        }
    }
    else {
        // Normal state
        Close_outline.setFillColor(sf::Color(0, 0, 0, 150)); // Light gray outline
        Close.setFillColor(sf::Color::White); // White button
        Close.setScale(1.0f, 1.0f); // Default scale for button
        Close_outline.setScale(1.0f, 1.0f); // Default scale for outline
    }


    
    // Restart button (no outline)
    if (Restart.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        Restart.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        Restart.setScale(1.1f, 1.1f); // Slightly larger scale for button

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Restart.setScale(1.05f, 1.05f); // Slightly smaller scale for button
        }
    }
    else {
        // Normal state
        Restart.setFillColor(sf::Color::White); // White button
        Restart.setScale(1.0f, 1.0f); // Default scale for button
    }

    // Resume button (back to game, no outline)
    if (resume.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        resume.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        resume.setScale(1.1f, 1.1f); // Slightly larger scale for button

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            resume.setScale(1.05f, 1.05f); // Slightly smaller scale for button
        }
    }
    else {
        // Normal state
        resume.setFillColor(sf::Color::White); // White button
        resume.setScale(1.0f, 1.0f); // Default scale for button
    }

    // Instructions button (no outline)
    if (Instructions.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        Instructions.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        Instructions.setScale(1.1f, 1.1f); // Slightly larger scale for button

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Instructions.setScale(1.05f, 1.05f); // Slightly smaller scale for button
        }
    }
    else {
        // Normal state
        Instructions.setFillColor(sf::Color::White); // White button
        Instructions.setScale(1.0f, 1.0f); // Default scale for button
    }

    // Exit button (no outline)
    if (exit.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Hover state
        exit.setFillColor(sf::Color(100, 255, 255)); // Light cyan button
        exit.setScale(1.1f, 1.1f); // Slightly larger scale for button

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            exit.setScale(1.05f, 1.05f); // Slightly smaller scale for button
            this->playClickSound(); // Play the click sound
        }
    }
    else {
        // Normal state
        exit.setFillColor(sf::Color::White); // White button
        exit.setScale(1.0f, 1.0f); // Default scale for button
    }

}


void UI::set_menu_objects(int width, int height)
{
    // Sci-fi Color Palette
    sf::Color titleColor(0, 255, 255);       // Bright Cyan
    sf::Color buttonTextColor(255, 255, 255);// Bright White
    sf::Color buttonOutlineColor(0, 255, 255, 180); // Translucent Cyan
    sf::Color buttonBgColor(0, 0, 0, 150);   // Semi-transparent Black    

    //sizes
    float baseHeight = 800.0f; // Reference height
    int titleSize = static_cast<int>(height * 50.0f / baseHeight); // Title character size
    int buttonSize = static_cast<int>(height * 30.0f / baseHeight);


    // Title
    title.setString("SPACE INVASION");
    title.setFont(font);
    title.setFillColor(titleColor);
    title.setOutlineColor(sf::Color(0, 255, 255, 100));
    title.setOutlineThickness(3);
    title.setCharacterSize(titleSize+10);

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
    play_button.setFillColor(buttonTextColor);
    play_button.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f, height * 0.5f); 

    // Play Button Outline (Center)
    play_button_outline.setSize(sf::Vector2f(play_button.getGlobalBounds().width + 50, play_button.getGlobalBounds().height + 20.f));
    play_button_outline.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f - 25, height * 0.5f - play_button.getGlobalBounds().height / 2.f + 20);
    play_button_outline.setOutlineColor(buttonOutlineColor);
    play_button_outline.setFillColor(buttonBgColor);
    play_button_outline.setOutlineThickness(3.0f);

    // Instructions Button (Bottom-left corner)
    instructions_button.setString("HOW TO PLAY");
    instructions_button.setFont(font);
    instructions_button.setCharacterSize(buttonSize);
    instructions_button.setFillColor(buttonTextColor);
    instructions_button.setPosition(0.05f * width, height * 0.9f);  // 5% from the left, 90% from the top

    instructions_button_outline.setPosition(0.05f * width - 20, height * 0.9f); // Outline adjustment to stay with the button
    instructions_button_outline.setSize(sf::Vector2f(instructions_button.getGlobalBounds().width + 40, instructions_button.getGlobalBounds().height + 30));
    instructions_button_outline.setOutlineColor(buttonOutlineColor);
    instructions_button_outline.setFillColor(buttonBgColor);
    instructions_button_outline.setOutlineThickness(3.0f);

    // Scores Button (Bottom-right corner)
    scores_button.setString("SCORES");
    scores_button.setFont(font);
    scores_button.setCharacterSize(buttonSize);
    scores_button.setFillColor(buttonTextColor);
    scores_button.setPosition(0.95f * width - scores_button.getGlobalBounds().width, height * 0.9f);  // 95% from the right, 90% from the top

    scores_button_outline.setPosition(0.95f * width - scores_button.getGlobalBounds().width - 20, height * 0.9f); // Outline adjustment to stay with the button
    scores_button_outline.setSize(sf::Vector2f(scores_button.getGlobalBounds().width + 40, scores_button.getGlobalBounds().height + 30));
    scores_button_outline.setOutlineColor(buttonOutlineColor);
    scores_button_outline.setFillColor(buttonBgColor);
    scores_button_outline.setOutlineThickness(3.0f);
    
    //Replay button
    Replay.setString("PlAY AGAIN");
    Replay.setFont(font);
    Replay.setFillColor(sf::Color::White);
    Replay.setCharacterSize(titleSize * 0.8f);

    Replay_outline.setFillColor(sf::Color::Black);
    Replay_outline.setOutlineColor(sf::Color::White);
    Replay_outline.setOutlineThickness(2.0f);
    Replay_outline.setSize(sf::Vector2f(Replay.getGlobalBounds().width + 20, Replay.getGlobalBounds().height + 20));

    //Exit Game button
    Close.setString("EXIT GAME");
    Close.setFont(font);
    Close.setFillColor(buttonTextColor);
    Close.setCharacterSize(buttonSize);

    Close_outline.setSize(sf::Vector2f(Close.getGlobalBounds().width + 12, Close.getGlobalBounds().height + 20));
    Close_outline.setOutlineColor(buttonOutlineColor);
    Close_outline.setFillColor(buttonBgColor);
    Close_outline.setOutlineThickness(3.0f);


    exit.setString("->EXIT GAME");
    exit.setFont(font);
    exit.setFillColor(sf::Color::White);
    exit.setCharacterSize(titleSize * 0.7f);

    //Gameover
    GameOver.setString("GAME OVER");
    GameOver.setFont(font);
    GameOver.setFillColor(sf::Color::Red);
    GameOver.setCharacterSize(titleSize+10);

    //game paused
    pause.setString("GAME PAUSED");
    pause.setFont(font);
    pause.setFillColor(sf::Color::Yellow);
    pause.setCharacterSize(titleSize + 10);

    //Score
    score.setString("SCORE: " + std::to_string(this->finalscore));
    score.setFont(font);
    score.setFillColor(sf::Color::Red);
    score.setCharacterSize(titleSize*0.6f);
    
    //restart
    Restart.setString("->RESTART GAME");
    Restart.setFont(font);
    Restart.setFillColor(sf::Color::White);
    Restart.setCharacterSize(titleSize * 0.7f);

    //back to game
    resume.setString("->MAIN_MENU");
    resume.setFont(font);
    resume.setFillColor(sf::Color::White);
    resume.setCharacterSize(titleSize * 0.7f);

    //instructions
    Instructions.setString("->GAME GUIDE");
    Instructions.setFont(font);
    Instructions.setFillColor(sf::Color::White);
    Instructions.setCharacterSize(titleSize * 0.7f);
    //set posiiton
    // Define margins and spacing
    const float verticalSpacing = 40.f; // Space between each element
    const float buttonOutlinePadding = 10.0f; // Padding for button outlines

    // Centering Game Over Text (center of screen)
    GameOver.setPosition(
        0.5f * width - GameOver.getGlobalBounds().width / 2.0f,
        0.4f * height - GameOver.getGlobalBounds().height
    );
    pause.setPosition(
        0.5f * width - GameOver.getGlobalBounds().width ,
        0.4f * height - GameOver.getGlobalBounds().height
    );

    // Placing the Score below Game Over Text
    score.setPosition(
        0.5f * width - score.getGlobalBounds().width / 2.0f,
        GameOver.getPosition().y + GameOver.getGlobalBounds().height + verticalSpacing
    );

    // Placing Replay Button below the Score
    Replay.setPosition(
        0.5f * width - Replay.getGlobalBounds().width / 2.0f,
        score.getPosition().y + score.getGlobalBounds().height + verticalSpacing
    );

    resume.setPosition(
       pause.getPosition().x,
        pause.getPosition().y + pause.getGlobalBounds().height + verticalSpacing
    );
    Restart.setPosition(
        pause.getPosition().x,
        resume.getPosition().y + resume.getGlobalBounds().height + verticalSpacing
    );
    Instructions.setPosition(
        pause.getPosition().x,
        Restart.getPosition().y + Restart.getGlobalBounds().height + verticalSpacing
    );
    exit.setPosition(
        pause.getPosition().x,
        Instructions.getPosition().y + Instructions.getGlobalBounds().height + verticalSpacing
    );

    // Placing Close Button below Replay Button
    Close.setPosition(0.05 * width , 0.05 * height);
      
    Close_outline.setPosition(0.05 * width - 10, 0.05 * height);

    music_button.setFont(font);
    music_button.setCharacterSize(buttonSize);
    

    if (isMusicOn) {
        music_button.setString("MUSIC:ON");
        music_button.setFillColor(buttonTextColor);
        music_button_outline.setSize(sf::Vector2f(music_button.getGlobalBounds().width + 12, music_button.getGlobalBounds().height + 20));
    }
    else {
        music_button.setString("MUSIC:OFF");
        music_button.setFillColor(sf::Color::Red);
        music_button_outline.setSize(sf::Vector2f(music_button.getGlobalBounds().width + 15, music_button.getGlobalBounds().height + 20));
    }

    // Set the outline properties
    music_button_outline.setOutlineColor(buttonOutlineColor);
    music_button_outline.setFillColor(buttonBgColor);
    music_button_outline.setOutlineThickness(3.0f);
    sf::FloatRect bounds = music_button.getGlobalBounds();
    music_button.setPosition(0.85 * width, 0.05 * height); // Top-right corner
    music_button_outline.setPosition(0.847*width , 0.055*height);
    
}




void UI::load_menu()
{
    window.draw(title);
    window.draw(play_button_outline);
    window.draw(play_button);
    window.draw(instructions_button_outline);
    window.draw(instructions_button);
    window.draw(scores_button_outline);
    window.draw(credits_button);
    window.draw(credits_button_outline);
    window.draw(scores_button);
    window.draw(Close_outline);
    window.draw(Close);
    window.draw(music_button);
    window.draw(music_button_outline);
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
    objectives.setString(
        "HOW TO PLAY: \n"
        "1. Use arrow keys to move your spaceship: \n"
        "   - UP arrow: Move up \n"
        "   - DOWN arrow: Move down \n"
        "   - LEFT arrow: Move left \n"
        "   - RIGHT arrow: Move right \n\n"
        "2. Press SPACE to shoot bullets and destroy enemies. \n"
        "3. Collect power-ups to enhance your abilities: \n"
        "   - Some power-ups increase your fire rate. \n"
        "   - Others provide shields to protect you from enemy attacks. \n\n"
        "4. Avoid enemy attacks! \n"
        "   - If you get hit by an enemy's bullet or collide with an enemy, you lose health. \n\n"
        "5. Destroy enemies to earn points: \n"
        "   - The more enemies you destroy, the higher your score! \n\n"
        "6. Boss battles: \n"
        "   - Defeat the boss to proceed to the next level. \n"
        "   - The boss has multiple stages, so be prepared for tougher challenges. \n\n"
        "7. Press ESC to pause the game at any time. \n\n"
        "Good luck, and may you save the galaxy!"
    );
    objectives.setFont(font);
    objectives.setFillColor(sf::Color::White);
    objectives.setCharacterSize(32);
    objectives.setPosition(0.15 * width, 300);

    // back button
    back_button.setString("BACK");
    back_button.setFont(font);
    back_button.setFillColor(sf::Color::White);
    back_button.setCharacterSize(35);
    back_button.setPosition(0.05 * width, 0.05 * height);

    back_button_outline.setFillColor(sf::Color::Black);
    back_button_outline.setOutlineColor(sf::Color::White);
    back_button_outline.setOutlineThickness(2.0f);
    back_button_outline.setSize(sf::Vector2f(back_button.getGlobalBounds().width + 20, back_button.getGlobalBounds().height + 20));
    back_button_outline.setPosition(0.05 * width - 10, 0.05 * height);
}

void UI::load_instructions()
{
    
      sf::Color currentColor = objectives.getFillColor();
        if (currentColor.a < 255) {
            currentColor.a += 5;  // Increase alpha for fade-in
            objectives.setFillColor(currentColor);
        }
        window.draw(how_to_play_title);
    window.draw(objectives);
    window.draw(back_button_outline);
    window.draw(back_button);
}

void UI::set_scores(int x)
{
   
}

void UI::load_score()
{
    this->window.draw(score);
}

void UI::initializeSounds() {
    if (!clickBuffer.loadFromFile("Sound/click.wav")) {
        std::cout << "Error: Could not load click sound" << std::endl;
    }
    clickSound.setBuffer(clickBuffer);
}

void UI::playClickSound() {
    clickSound.play();
}

void UI::set_credits_objects(int width, int height) {
    // Credits title
    credits_title.setString("CREDITS");
    credits_title.setFont(font);
    credits_title.setFillColor(sf::Color::White);
    credits_title.setCharacterSize(72);
    credits_title.setPosition(width / 2 - credits_title.getGlobalBounds().width / 2, 0.1 * height);

    // Credits text
    credits_text.setString(
        "SPACE INVASION\n\n"
        "Game Development Team:\n\n"
        "Programming:\n"
        "- Lead Programmer: [Your Name]\n"
        "- Game Engine: SFML\n\n"
        "Art & Design:\n"
        "- Character Design: [Artist Name]\n"
        "- UI Design: [Designer Name]\n\n"
        "Sound & Music:\n"
        "- Sound Effects: [Sound Designer]\n"
        "- Background Music: [Composer]\n\n"
        "Special Thanks:\n"
        "- Beta Testers\n"
        "- SFML Community\n"
        "- Our Families and Friends\n\n"
        "© 2024 Space Invasion Team\n"
        "All Rights Reserved"
    );

    credits_text.setFont(font);
    credits_text.setFillColor(sf::Color::White);
    credits_text.setCharacterSize(32);
    credits_text.setPosition(0.15 * width, 300);

    // Back button (reuse existing back button properties)
    // The back button is already defined in the class
}

void UI::load_credits() {
    sf::Color currentColor = credits_text.getFillColor();
    if (currentColor.a < 255) {
        currentColor.a += 5;  // Increase alpha for fade-in
        credits_text.setFillColor(currentColor);
    }

    window.draw(credits_title);
    window.draw(credits_text);
    window.draw(back_button_outline);
    window.draw(back_button);
}


void UI::load_end_page()
{
}

void UI::load_game()
{
}

void UI::pause_page()
{
    window.draw(back_button_outline);
    window.draw(back_button);
    window.draw(pause);
    window.draw(resume);
    window.draw(Restart);
    window.draw(Instructions);
    window.draw(exit);
    window.draw(music_button);
    window.draw(music_button_outline);
}
