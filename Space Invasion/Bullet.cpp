#include "Bullet.h"
#include<cmath>
Bullet::Bullet(sf::Texture* texture, float posx, float posy, float dx, float dy, float movement_Speed, int damage,int level) :damage(damage)
{
	this->shape.setTexture(*texture);
	float factor;
	factor = std::min(0.015f, 0.01f+level * 0.0005f);
	this->shape.scale(factor, factor);

	this->shape.setPosition(posx, posy);
	this->direction.x = std::min(10.f,dx * movement_Speed);
	this->direction.y = std::min(10.f, dy * movement_Speed);

};

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
	this->shape.move(this->direction.x,this->direction.y);

}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);

}
