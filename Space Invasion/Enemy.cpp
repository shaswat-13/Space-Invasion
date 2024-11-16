#include "Enemy.h"
#include "eship.h"
#include<random>
// Initialize variables (type, hp, damage, etc.)
void Enemy::initVariables()
{
    this->pointCount = 10;
    this->speed = 0.5f;
    this->type = 0;
    this->hp = this->hpMax;
    this->hpMax = static_cast<int>(this->pointCount);
    this->damage = 3;
    this->bulletDamage = 1;
    this->points = 5;
    this->shootCooldownMax = 120.f;
    this->shootCooldown = this->shootCooldownMax;
}

// Load the texture and handle potential errors
void Enemy::initShape()
{
    if (!this->texture.loadFromMemory(space1_png,space1_png_len))
    {
        std::cerr << "ERROR::ENEMY::Failed to load texture from file" << std::endl;
    }
}

// Initialize the sprite with the loaded texture


void Enemy::initSprite()
{
    // Define the scale range (e.g., between 0.5 and 2.0)
    const float minScale = 0.01f;
    const float maxScale = 0.1f;



    float scaleFactor = 0.1f;

    this->EnemyShips.setTexture(this->texture);

    // Resize the sprite using the random scale factor
    this->EnemyShips.scale(scaleFactor, scaleFactor);
}


// Constructor with position validation
Enemy::Enemy(float posx, float posy)
{
    this->initVariables();
    this->initShape();
    this->initSprite();

    this->validatePosition(posx, posy);
    this->EnemyShips.setPosition(posx, posy);
}

bool Enemy::canShoot() {
    if (this->shootCooldown >= this->shootCooldownMax) {
        this->shootCooldown = 0.f; // Reset timer after shooting
        return true;
    }
    this->shootCooldown += 1.f; // Increment timer
    return false;
}

void Enemy::update() {
    this->EnemyShips.move(0.f, speed);
}

// Destructor (currently empty, but useful for future expansions)
Enemy::~Enemy()
{

}

const sf::Vector2f& Enemy::getPos() const
{
    return this->EnemyShips.getPosition();
}

// Function to validate and adjust the enemy's initial position if necessary
void Enemy::validatePosition(float& posx, float& posy)
{
    // Define the screen bounds
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    // Adjust position if it's outside the screen bounds
    if (posx < 0.0f) posx = 0.0f;
    if (posy < 0.0f) posy = 0.0f;
    if (posx > screenWidth - this->EnemyShips.getGlobalBounds().width)
        posx = screenWidth - this->EnemyShips.getGlobalBounds().width;
    if (posy > screenHeight - this->EnemyShips.getGlobalBounds().height)
        posy = screenHeight - this->EnemyShips.getGlobalBounds().height;
}

const sf::FloatRect Enemy::getBounds() const
{
    return this->EnemyShips.getGlobalBounds();
}




void Enemy::takeDamage(int damage) {
    this->hp -= damage;
    if (this->hp < 0) this->hp = 0;
}

const int& Enemy::getPoints() const
{
    // TODO: insert return statement here
    return this->points;
}

const int& Enemy::getDamage() const
{
    return this->damage;
}

const int& Enemy::getBulletDamage() const
{
    return this->bulletDamage;
}


bool Enemy::isDead() const {
    return this->hp <= 0;
}



// Render the enemy on the screen
void Enemy::render(sf::RenderTarget* target)
{
    if (target)
    {
        target->draw(this->EnemyShips);
    }
}




















