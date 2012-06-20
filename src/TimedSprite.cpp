#include "..\include\TimedSprite.h"

#include "SMFFETime.h"

TimedSprite::TimedSprite(const sf::Vector2f& pPosition, float pTime) :
	AnimatedSprite(pPosition), mTimeBeforeDeletion(pTime)
{

}


TimedSprite::~TimedSprite(void)
{
}

void TimedSprite::update()
{
	mTimeBeforeDeletion -= Time::deltaTime();

	if(mTimeBeforeDeletion < 0)
		mToDelete = true;

	AnimatedSprite::update();
}