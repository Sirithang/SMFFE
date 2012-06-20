#ifndef SMFFE_PROJECTILE_H
#define SMFFE_PROJECTILE_H

#include "animatedsprite.h"

class Projectile :
	public AnimatedSprite
{
protected:
	sf::Vector2f	mDirection;
	float			mSpeed;

public:
	Projectile(sf::Vector2f pPosition, sf::Vector2f pDirection, float pSpeed);
	~Projectile(void);

	void update();
};

#endif
