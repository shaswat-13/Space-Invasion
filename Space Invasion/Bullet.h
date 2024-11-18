#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include<cmath>
class Bullet
{
private:
    sf::Sprite shape;
    sf::Texture* texture;
    sf::Vector2f direction;
    float movementSpeed;
    int damage;
    float angle; // Angle for rotation and movement direction

public:
    Bullet(sf::Texture* texture, float posx, float posy, float dx, float dy, float movement_Speed, int damage,int level) ;
   
    virtual ~Bullet();

    // Accessors
    const sf::FloatRect getBounds() const;
    int getdamage();

    // Functions
    void update();
    void render(sf::RenderTarget* target);
};

#endif // BULLET_H
