#ifndef ENEMY_H 
#define ENEMY_H

#include<SFML/Graphics.hpp>
#include<iostream>

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
    void initShape();
    void initVariables();
    void initSprite();

    void validatePosition(float& posx, float& posy);

public:
    Enemy(float posx, float posy);
    virtual ~Enemy();
    const sf::Vector2f& getPos() const;

    //Accessors
    const sf::FloatRect getBounds() const;
    void takeDamage(int damage);
    bool isDead() const;

    bool canShoot();
    const int& getPoints() const;
    const int& getDamage() const;
    const int& getBulletDamage()const;
    // Functions
    void update();
    void render(sf::RenderTarget* target);
};

#endif //ENEMY_H
