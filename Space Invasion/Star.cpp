// Star.cpp
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

void Star::update(float levelSpeedMultiplier)
{
    // Update the twinkle effect by modifying the scale
    float time = std::sin(this->twinkleOffset + (std::clock() / 1000.0f) * this->twinkleSpeed);
    float scale = 0.1f + 0.5f * time; // Scale between 0.5 and 1.0
    this->shape.setScale(scale, scale);

    // Move the star downwards, adjusting speed with level multiplier
    float adjustedSpeed =levelSpeedMultiplier;
    this->shape.move(0.f, adjustedSpeed);

    // Wrap around the screen
    if (this->shape.getPosition().x < 0.f) this->shape.setPosition(this->shape.getPosition().x + 800.f, this->shape.getPosition().y); // Wrap around horizontally
    if (this->shape.getPosition().x > 800.f) this->shape.setPosition(this->shape.getPosition().x - 800.f, this->shape.getPosition().y); // Wrap around horizontally
    if (this->shape.getPosition().y < 0.f) this->shape.setPosition(this->shape.getPosition().x, this->shape.getPosition().y + 600.f); // Wrap around vertically
    if (this->shape.getPosition().y > 600.f) this->shape.setPosition(this->shape.getPosition().x, this->shape.getPosition().y - 600.f); // Wrap around vertically
}

void Star::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
}
