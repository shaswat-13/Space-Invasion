#pragma once
#include <SFML/Graphics.hpp>

class Star {
public:
    Star(float x, float y, float radius);

    // Updated to accept level speed multiplier as a parameter
    void update(float levelSpeedMultiplier);
    void render(sf::RenderTarget& target);

private:
    sf::CircleShape shape;
    float baseSpeed;       // Base speed, adjusted in `update` with level multiplier
    float twinkleSpeed;    // Twinkle speed
    float twinkleOffset;   // Initial twinkle phase offset
};
