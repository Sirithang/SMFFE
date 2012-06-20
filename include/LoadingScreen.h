#ifndef SMFFE_LOADINGSCREEN_H
#define SMFFE_LOADINGSCREEN_H

#include "ascreen.h"

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Sprite.h"

class LoadingScreen :
	public AScreen
{
protected:
	Player* mPlayer1;
	Player* mPlayer2;

	Sprite	mBackGround;
	Sprite  mNyan;
	
	sf::Font mFont;
	sf::Text mPressButton;

	sf::SoundBuffer mBufferValidation;
	sf::Sound mValidation;

	sf::SoundBuffer mEndLoading;
	sf::Sound		mLoading;
	bool			mLoadEnded;

	float mPurcentage;

public:
	LoadingScreen(Player* pPlayer1, Player* pPlayer2);
	~LoadingScreen(void);

	void update();
	void draw(sf::RenderTarget* pTarget);
};

#endif