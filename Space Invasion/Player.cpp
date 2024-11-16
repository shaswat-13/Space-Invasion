
#include "Player.h"

#include "ship.h"
void Player::initVariables()
{
	this->movementSpeed = 2.f;
	this->attackCooldownmax = 10.f;
	this->attackCooldown = this->attackCooldownmax;
	this->hpmax = 10;
	this->hp = this->hpmax;


}

void Player::initTexture()
{
	if (!this->texture.loadFromMemory(space2_png,space2_png_len))
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
	this->sprite.scale(0.1f, 0.1f);



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

void Player::losehp(int damage)
{
	this->hp -= damage;
	if (this->hp < 0)
		this->hp = 0;

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

		return true;
	}
	else
		return false;
}

void Player::updateAttack()
{

	if (this->attackCooldown < this->attackCooldownmax)
		this->attackCooldown += 0.5f;

}

//function 
void Player::update()
{
	this->updateAttack();
}

void Player::resetstats()
{
	this->hp = this->hpmax;
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
