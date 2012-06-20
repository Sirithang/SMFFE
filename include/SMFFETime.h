#ifndef SMFFE_TIME_H
#define SMFFE_TIME_H

#include <SFML/System/Clock.hpp>

class Time
{
private:

	static Time* sInstance;

	sf::Clock mClock;

	float mLastTime;
	float mDeltaTime;

	Time(void);
	~Time(void);

public:
	static void tick();
	static float deltaTime() {return sInstance->mDeltaTime;}
};

#endif
