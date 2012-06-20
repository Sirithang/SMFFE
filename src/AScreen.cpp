#include "..\include\AScreen.h"
#include "SMFFEConfig.h"

AScreen::AScreen(void)
{
	mIsFrozen = false;
	mView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(SMFFEConfig::instance().getViewWidth(), SMFFEConfig::instance().getViewHeight()));
}


AScreen::~AScreen(void)
{
}
