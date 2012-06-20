#include "..\include\SMFFETime.h"

Time* Time::sInstance = new Time();

Time::Time(void)
{
	mLastTime = 0;
	mDeltaTime = 0;
}


Time::~Time(void)
{
}

//*************************************

void Time::tick()
{
	sInstance->mDeltaTime = sInstance->mClock.GetElapsedTime()*0.001 - sInstance->mLastTime;
	sInstance->mLastTime = sInstance->mClock.GetElapsedTime()*0.001;
}
