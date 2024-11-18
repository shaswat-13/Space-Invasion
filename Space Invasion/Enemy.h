#ifndef ENEMY_H 
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
private:
    int type;
    float speed;
    unsigned pointCount;
    int hp;
    int hpMax;
    int damage;
    int bulletDamage;
    int points;
    sf::Sprite EnemyShips;
    sf::Sprite spaceship;
    sf::Texture texture;
    sf::Texture shiptext;
    float shootCooldownMax;  // Max cooldown for shooting
    float shootCooldown;     // Current cooldown timer

    float rotation;  // Enemy rotation towards the player

    void initShape();
    void initVariables();
    void initSprite();
    void validatePosition(float& posx, float& posy);

public:
    Enemy(float posx, float posy);
    virtual ~Enemy();

    const sf::Vector2f& getPos() const;
    const sf::Vector2f getCenter() const; // Returns the center of the enemy sprite


    // Accessors
    const sf::FloatRect getBounds() const;
    void takeDamage(int damage);
    bool isDead() const;

    void updateattack(int x);
    const bool canShoot();
    const int& getPoints() const;
    const int& getDamage() const;
    const int& getBulletDamage() const;

    // Functions
    void update(float level);
    void render(sf::RenderTarget* target);
};

#endif // ENEMY_H
