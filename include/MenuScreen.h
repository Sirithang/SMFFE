#ifndef SMFFE_MENUSCREEN_H
#define SMFFE_MENUSCREEN_H

#include <SFML/Audio.hpp>

#include "ascreen.h"
#include "Sprite.h"

#include "Player.h"

class MenuScreen :
	public AScreen
{
protected:
	Sprite mBanner;

	Sprite mBackground;

	Sprite mVersusNormal;
	Sprite mVersusHighlight;
	Sprite mTrainingNormal;
	Sprite mTrainingHighlight;
	Sprite mOptionNormal;
	Sprite mOptionHighlight;
	Sprite mCreditNormal;
	Sprite mCreditHighlight;

	Sprite* mVersus;
	Sprite* mTraining;
	Sprite* mOption;
	Sprite* mCredit;

	int mNbChoice;
	int mCurrentChoice;

	sf::Music* mMusic;

	sf::SoundBuffer mBufferNavigation;
	sf::SoundBuffer mBufferValidation;

	int mCurrentNavigation;
	sf::Sound mNavigation[3];
	sf::Sound mValidation;

	Player* mFirstPlayer;

	void moveSelector();
	void gotoChoice();

public:
	MenuScreen(CommandInput* pInput, sf::Music* pMusic = NULL);
	~MenuScreen(void);

	void update();
	void draw(sf::RenderTarget* pTarget);

	
};

#endif

