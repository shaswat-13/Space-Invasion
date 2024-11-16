#include "Bullet.h"
Bullet::Bullet()
{

}
Bullet::Bullet(sf::Texture* texture, float posx, float posy, float dx, float dy, float movement_Speed, int damage) :damage(damage)
{
	this->shape.setTexture(*texture);
	this->shape.scale(0.01f, 0.01f);

	this->shape.setPosition(posx, posy);
	this->direction.x = dx;
	this->direction.y = dy;
	this->movementSpeed = movement_Speed;

}
Bullet::~Bullet()
{

}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

int Bullet::getdamage()
{
	return this->damage;
}

void Bullet::update()
{
	//movement
	this->shape.move(this->movementSpeed * direction);

}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);

}
