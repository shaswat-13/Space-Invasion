#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

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


    void losehp(int damage);

    void move(const float dx, const float dy, const sf::RenderWindow& window);
    const bool canAttack();
    void updateAttack();
    void update();
    void resetstats();
    void render(sf::RenderTarget& target);
};
