
#ifndef BULLET_H
#define BULLET_H
#include<SFML/Graphics.hpp>
#include<iostream>

#define BULLET_H
class Bullet
{
private:

	sf::Sprite shape;
	sf::Texture* texture;
	sf::Vector2f direction;
	float movementSpeed;
	int damage;
	int maxdamage;
public:

	Bullet();
	Bullet(sf::Texture* texture, float posx, float posy, float dx, float dy, float movement_Speed, int damage);
	virtual ~Bullet();


	//Accessor
	const sf::FloatRect getBounds() const;

	int getdamage();
	void update();
	void render(sf::RenderTarget* target);
};

#endif // !BULLET_H