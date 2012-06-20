#ifndef SMFFE_SPLASHSCREEN_H
#define SMFFE_SPLASHSCREEN_H

#include "ascreen.h"
#include "Sprite.h"

class SplashScreen :
	public AScreen
{
protected:
	Sprite mLogo;
	float mTimeSinceStart;

public:
	SplashScreen(void);
	~SplashScreen(void);

	void update();
	void draw(sf::RenderTarget* pTarget);
};

#endif