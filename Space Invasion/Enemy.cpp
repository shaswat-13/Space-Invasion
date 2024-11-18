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
    this->damage = 1;
    this->bulletDamage = 1;
    this->points = 5;
    this->shootCooldownMax = 10.f;
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

const bool Enemy::canShoot()
{
    if (this->shootCooldown >= this->shootCooldownMax)
    {
        this->shootCooldown = 0.f;

        return true; //allow attack
    }
    return false; // prevent attack if cooldown is not finished
}

void Enemy::updateattack(int x)
{
    // Calculate the max cooldown based on the level (x), using an exponential decay for gradual reduction
    this->shootCooldownMax = std::max(3.0f, 10.0f / (1 + std::exp(-0.2f * (x - 10)))); // Exponential decay after level 10

    // Gradually decrease the attackCooldown towards 0, ensuring it's capped at attackCooldownmax
    if (this->shootCooldown < this->shootCooldownMax) {
        this->shootCooldown += 0.03f * x;  // This increases cooldown towards attackCooldownmax
        if (this->shootCooldown > this->shootCooldownMax) {
            this->shootCooldown = this->shootCooldownMax; // Cap it at attackCooldownmax
        }
    }
    else if (this->shootCooldown > 0) {
        // Gradually decrease the cooldown to 0 (allow firing)
        this->shootCooldown -= 0.05f * x; // Slow decrease towards 0
        if (this->shootCooldown < 0) {
            this->shootCooldown = 0; // Cap it at 0 to allow firing
        }
    }
}



void Enemy::update(float level) {
    this->speed=std::min(8.f,0.7f*level);
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

const sf::Vector2f Enemy::getCenter() const
{
    return sf::Vector2f(
        this->EnemyShips.getPosition().x + this->EnemyShips.getGlobalBounds().width / 2.f,
        this->EnemyShips.getPosition().y + this->EnemyShips.getGlobalBounds().height / 2.f
    );
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

















