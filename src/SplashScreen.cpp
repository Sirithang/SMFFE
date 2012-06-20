#include "..\include\SplashScreen.h"

#include "SMFFETime.h"
#include "StartScreen.h"
#include "SMFFEConfig.h"

SplashScreen::SplashScreen(void)
	:mLogo("data/screens/startScreen/SplashScreen.png", sf::Vector2f(0,0))
{
	mLogo.getSFSprite().SetColor(sf::Color(255, 255, 255, 0));
	mTimeSinceStart = 0;

	mView = sf::View(	sf::Vector2f(	SMFFEConfig::instance().getViewWidth() * 0.5f,
										SMFFEConfig::instance().getViewHeight() * 0.5f),
						sf::Vector2f(SMFFEConfig::instance().getViewWidth(), SMFFEConfig::instance().getViewHeight()));
}


SplashScreen::~SplashScreen(void)
{
}

void SplashScreen::update()
{
	mTimeSinceStart += Time::deltaTime();

	if(mTimeSinceStart < 4.0f)
	{
		mLogo.getSFSprite().SetColor(sf::Color(255, 255, 255, mTimeSinceStart/4.0f * 255));
	}
	else if(mTimeSinceStart < 8.0f)
	{
		mLogo.getSFSprite().SetColor(sf::Color(255, 255, 255, (1.0f - ((mTimeSinceStart - 4.0f)/4.0f)) * 255));
	}
	else
	{
		mManager->popScreen();
		mManager->pushScreen(new StartScreen());
	}
}

void SplashScreen::draw(sf::RenderTarget* pTarget)
{
	pTarget->Clear();
	mLogo.draw(pTarget);
}