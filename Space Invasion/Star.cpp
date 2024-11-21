#include "Star.h"
#include <cmath>

Star::Star(float x, float y, float radius)
{
    this->shape.setRadius(radius);
    this->shape.setFillColor(sf::Color::White);
    this->shape.setPosition(x, y);
    this->shape.setOrigin(radius, radius); // Center the circle

    // Initialize twinkle effect parameters
    this->twinkleSpeed = static_cast<float>(rand()) / RAND_MAX * 0.1f;
    this->twinkleOffset = static_cast<float>(rand()) / RAND_MAX * 360.0f; // Random initial twinkle phase

    // Set a consistent downward movement, with speed to be adjusted based on level
    this->baseSpeed = 0.1f; // Base speed, adjustable with level later
}

void Star::update(float levelSpeedMultiplier, const sf::RenderWindow& window)
{
    // Update the twinkle effect by modifying the scale
    float time = std::sin(this->twinkleOffset + (std::clock() / 1000.0f) * this->twinkleSpeed);
    float scale = 0.1f + 0.5f * time; // Scale between 0.5 and 1.0
    this->shape.setScale(scale, scale);

    // Get window size dynamically for responsive scaling
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);

    // Adjust star speed based on the level and window size
    float adjustedSpeed = levelSpeedMultiplier * windowHeight / 600.f;  // Scale speed based on height of the window

    // Move the star downwards, adjusting speed with level multiplier
    this->shape.move(0.f, adjustedSpeed);

    // Wrap around the screen, using the dynamic window size
    if (this->shape.getPosition().x < 0.f) this->shape.setPosition(this->shape.getPosition().x + windowWidth, this->shape.getPosition().y); // Wrap around horizontally
    if (this->shape.getPosition().x > windowWidth) this->shape.setPosition(this->shape.getPosition().x - windowWidth, this->shape.getPosition().y); // Wrap around horizontally
    if (this->shape.getPosition().y < 0.f) this->shape.setPosition(this->shape.getPosition().x, this->shape.getPosition().y + windowHeight); // Wrap around vertically
    if (this->shape.getPosition().y > windowHeight) this->shape.setPosition(this->shape.getPosition().x, this->shape.getPosition().y - windowHeight); // Wrap around vertically
}

void Star::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
}
