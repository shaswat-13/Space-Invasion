#include "Enemy.h"
#include "eship.h"
#include"boss.h"
#include<random>



Enemy::Enemy(float posx, float posy, float level, bool isBoss)
    :type(isBoss ? Boss : Minion), level(level)
{

    this->initVariables(isBoss);
    this->initShape(isBoss);
    this->initSprite(isBoss);

    this->validatePosition(posx, posy);
    if (isBoss)
        this->boss.setPosition(posx, posy);
    else
        this->EnemyShips.setPosition(posx, posy);

}



// Initialize variables (type, hp, damage, etc.)
void Enemy::initVariables(bool isBoss)
{
    
    if (isBoss) {
        // Boss color
        this->speed = 0.5f;   // Higher health for boss
        this->hpMax = 5000.f;
        // Slower for boss
        this->hp = this->hpMax;
        this->damage = 10.f; // Boss damage
        this->points = 50;
    
    }
    else {
        // Minion color
        this->speed = 1.f + 0.1f * level;                    // Faster minions
        this->damage = 2;
        this->points = 5;
    }
}

bool Enemy::isDestroyed()
{
    if (this->hp == 0)
        return true;
    else
        return false;
}

// Load the texture and handle potential errors
void Enemy::initShape(bool isBoss)
{
    if (isBoss)
    {
        if (!this->bossText.loadFromMemory(boss_png, boss_png_len))
        {
            std::cerr << "ERROR::ENEMY::Failed to load Boss from file" << std::endl;

        }
        

        this->boss.setTexture(this->bossText);
       
    }
    else
    {
        if (!this->texture.loadFromMemory(space1_png, space1_png_len))
        {
            std::cerr << "ERROR::ENEMY::Failed to load texture from file" << std::endl;
        }
        this->EnemyShips.setTexture(this->texture);

        
    }
}

// Initialize the sprite with the loaded texture


void Enemy::initSprite(bool isBoss)
{
    if (isBoss)
    {
       
        this->boss.scale(0.1f, 0.1f);

    }
    else
    {
        float scaleFactor = 0.1f;

        this->EnemyShips.scale(scaleFactor, scaleFactor);
    }
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

const bool Enemy::bosscanShoot()
{
    // Boss shoots continuously, but checks if the cooldown is finished before firing
    if (this->shootCooldown >= this->shootCooldownMax) {
        this->shootCooldown = 0.f;  // Reset the cooldown
        return true;                 // Allow the boss to shoot
    }

    return false; // Boss cannot shoot yet
}

void Enemy::updateBossAttack()
{
    // Set cooldown between shots to 0.1 seconds
    this->shootCooldownMax = 0.1f;

    // Gradually increase the cooldown (to allow next shot)
    if (this->shootCooldown < this->shootCooldownMax) {
        this->shootCooldown += 0.016f; // Increment cooldown per frame (~1 frame at 60 FPS)
    }

    // The boss can keep shooting continuously once the cooldown resets
}

void Enemy::update(float level) {
    this->speed=std::min(8.f,0.7f*level);
    this->EnemyShips.move(0.f, speed);
    this->boss.move(0.f, 1.f);
    
}


// Destructor (currently empty, but useful for future expansions)
Enemy::~Enemy()
{

}

const sf::Vector2f& Enemy::getPos(bool isBoss) const
{
    if (!isBoss)
        return this->EnemyShips.getPosition();
    else
        return this->boss.getPosition();
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
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    // Define the screen bounds
    const float screenWidth = desktopMode.width;
    const float screenHeight = desktopMode.height;

    // Adjust position if it's outside the screen bounds
    if (posx < 0.0f) posx = 0.0f;
    if (posx > screenWidth - this->EnemyShips.getGlobalBounds().width)
        posx = screenWidth - this->EnemyShips.getGlobalBounds().width;
  
}

const sf::FloatRect Enemy::getBounds(bool isBoss) const
{
    if (isBoss)
        return this->boss.getGlobalBounds();
    else
        return this->EnemyShips.getGlobalBounds();
}




void Enemy::takeDamage(float damage) {
    this->hp = this->hp - damage;
    if (this->hp <= 0.f) {
        this->hp = 0.f;
    }
  
}

const int& Enemy::getPoints() const
{
    // TODO: insert return statement here
    return this->points;
}

const float& Enemy::getDamage() const
{
    return this->damage;
}

float Enemy::gethp()
{
    return this->hp;
}

float Enemy::gethpmax()
{
    return this->hpMax;
}

const int& Enemy::getBulletDamage() const
{
    return this->bulletDamage;
}


bool Enemy::isDead() const {
    return this->hp <= 0;
}




// Render the enemy on the screen
void Enemy::renderminion(sf::RenderTarget* target)
{
    if (target)
    {
        target->draw(this->EnemyShips);
    }
}

void Enemy::renderboss(sf::RenderTarget* target)
{
    if (target)
    {
        target->draw(this->boss);
    }
    
}

















