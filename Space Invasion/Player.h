#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include<cmath>

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    float movementSpeed;
    float attackCooldown;
    float attackCooldownmax;

    float hp;
    float hpmax;
    float damage;

    void initVariables();
    void initTexture();
    void initSprite();
    

public:
    
    Player();
    virtual ~Player();

    const sf::Vector2f& getPos() const;
    const sf::FloatRect getBounds() const
    {
        return this->sprite.getGlobalBounds();
    }
    const float& getHp() const;

    const float& getHpMax() const;
    sf::Sprite& getSprite() {
        return this->sprite;
    }

    void sethpmax();
    void losehp(int damage);
    float give_damage();
    void setpos(int height, int width);
    void move(const float dx, const float dy, const sf::RenderWindow& window);
    const bool canAttack();
    void updateAttack(int x);
    void update(int x);
    void resetstats();
    void render(sf::RenderTarget& target);
    void pulsating(int alpha);
};
