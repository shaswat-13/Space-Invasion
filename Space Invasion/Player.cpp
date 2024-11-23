#include "Player.h"

#include "ship.h"
void Player::initVariables()
{
	this->movementSpeed = 2.f;
	this->attackCooldownmax = 10.f;
	this->attackCooldown = this->attackCooldownmax;
	this->hpmax = 500;
	this->hp = this->hp;
	this->damage = 10.f;


}

void Player::initTexture()
{
	if (!this->texture.loadFromMemory(space2_png, space2_png_len))
	{
		std::cout << "Something went wrong:Player" << "\n";
	}

	//load texture from file
}

void Player::initSprite()
{
	//set texture to sprite
	this->sprite.setTexture(this->texture);

	// resize
	this->sprite.scale(0.13f, 0.13f);



}

Player::Player()
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
}

Player::~Player()
{

}

const sf::Vector2f& Player::getPos() const
{

	return this->sprite.getPosition();

}

const float& Player::getHp() const
{
	return this->hp;
}

const float& Player::getHpMax() const
{
	return this->hpmax;
}

void Player::sethpmax()
{
	this->hp = 500;
}

void Player::losehp(int damage)
{
	this->hp -= damage;
	if (this->hp < 0)
		this->hp = 0;

}

float Player::give_damage()
{
	return this->damage;
}

void Player::setpos(int width, int height)
{
	this->sprite.setPosition(width,height);

}

void Player::move(const float dx, const float dy, const sf::RenderWindow& window)
{
	// Calculate the new position based on movement speed
	sf::Vector2f newPos = this->sprite.getPosition() + sf::Vector2f(this->movementSpeed * dx, this->movementSpeed * dy);

	// Get the window dimensions
	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;

	// Constrain movement within the window bounds dynamically
	if (newPos.x < 0)
		newPos.x = 0;
	if (newPos.x > windowWidth - this->sprite.getGlobalBounds().width)
		newPos.x = windowWidth - this->sprite.getGlobalBounds().width;
	if (newPos.y < 0)
		newPos.y = 0;
	if (newPos.y > windowHeight - this->sprite.getGlobalBounds().height)
		newPos.y = windowHeight - this->sprite.getGlobalBounds().height;

	// Set the constrained position
	this->sprite.setPosition(newPos);
}


const bool Player::canAttack()
{
	if (this->attackCooldown >= this->attackCooldownmax)
	{
		this->attackCooldown = 0.f;

		return true; //allow attack
	}
	return false; // prevent attack if cooldown is not finished
}

void Player::updateAttack(int x)
{
	// Calculate the max cooldown based on the level (x), using an exponential decay for gradual reduction
	this->attackCooldownmax = std::max(1.0f, 10.0f / (1 + std::exp(-0.2f * (x - 10)))); // Exponential decay after level 10

	// Gradually decrease the attackCooldown towards 0, ensuring it's capped at attackCooldownmax
	if (this->attackCooldown < this->attackCooldownmax) {
		this->attackCooldown += 0.1f * x;  // This increases cooldown towards attackCooldownmax
		if (this->attackCooldown > this->attackCooldownmax) {
			this->attackCooldown = this->attackCooldownmax; // Cap it at attackCooldownmax
		}
	}
	else if (this->attackCooldown > 0) {
		// Gradually decrease the cooldown to 0 (allow firing)
		this->attackCooldown -= 0.05f * x; // Slow decrease towards 0
		if (this->attackCooldown < 0) {
			this->attackCooldown = 0; // Cap it at 0 to allow firing
		}
	}
}





//function 
void Player::update(int x)
{
	this->movementSpeed = std::min(5.f, 2.f + 0.5f * x);
	this->damage = std::min((float)x * 10.0f, 500.0f);  // For example, cap damage at 500
	this->hpmax = 500 + 50 * x;

}

void Player::resetstats()
{
	this->hp = this->hpmax;
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

void Player::pulsating(int alpha)
{

}