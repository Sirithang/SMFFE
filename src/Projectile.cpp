#include "..\include\Projectile.h"

#include "SMFFETime.h"

Projectile::Projectile(sf::Vector2f pPosition, sf::Vector2f pDirection, float pSpeed)
	:AnimatedSprite(pPosition), mDirection(pDirection), mSpeed(pSpeed)
{
}


Projectile::~Projectile(void)
{
}

//---------------------------------------------------

void Projectile::update()
{
	setPosition(getPosition() + mDirection * mSpeed * Time::deltaTime());
	AnimatedSprite::update();
}