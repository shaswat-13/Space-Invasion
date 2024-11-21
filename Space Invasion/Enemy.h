#ifndef ENEMY_H 
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
private:
    enum EnemyType { Minion, Boss };
    EnemyType type;
    float speed;
    unsigned pointCount;
    float hp;
    float hpMax;
    int damage;
    int bulletDamage;
    int points;
    int level;
    sf::Sprite EnemyShips;
    sf::Sprite boss;
    sf::Texture texture;
    sf::Texture bossText;
    float shootCooldownMax;  // Max cooldown for shooting
    float shootCooldown;     // Current cooldown timer

    void initShape(bool isBoss);
    void initVariables(bool isBoss);
    void initSprite(bool isBoss);
    void validatePosition(float& posx, float& posy);

public:
    Enemy(float posx, float posy, float level, bool isBoss = false);   
    virtual ~Enemy();

    const sf::Vector2f& getPos(bool isBoss) const;
    const sf::Vector2f getCenter() const; // Returns the center of the enemy sprite


    // Accessors
    const sf::FloatRect getBounds() const;
    void takeDamage(int damage);
    bool isDead() const;

    void updateattack(int x);
    const bool canShoot();
    const int& getPoints() const;
    const int& getDamage() const;
    float gethp();
    float gethpmax();
    const int& getBulletDamage() const;

    // Functions
    void update(float level);
    void renderminion(sf::RenderTarget* target);
    void renderboss(sf::RenderTarget* target);
};

#endif // ENEMY_H
