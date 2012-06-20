#ifndef SMFEE_TIMEDSPRITE_H
#define SMFEE_TIMEDSPRITE_H

#include "animatedsprite.h"

class TimedSprite :
	public AnimatedSprite
{
protected:
	float mTimeBeforeDeletion;

public:
	TimedSprite(const sf::Vector2f& pPosition, float pTime);
	~TimedSprite(void);

	void update();

	float getRemainingTime() { return mTimeBeforeDeletion; }
};

#endif
