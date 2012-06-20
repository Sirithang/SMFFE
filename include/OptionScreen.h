#ifndef SMFFE_OPTIONSCREEN_H
#define SMFFE_OPTIONSCREEN_H

#include "ascreen.h"

#include "Player.h"

class OptionScreen :
	public AScreen
{
protected:
	Player* mPlayer;

	Sprite mBackground;

	sf::Font mFont;
	sf::Text mTitle;

	sf::Text mResolution;
	sf::Text mFullscreen;
	sf::Text mMusicTxt;

	sf::Text mMapping;
	sf::Text mLP;
	sf::Text mHP;
	sf::Text mLK;
	sf::Text mHK;

	Sprite	mFlecheSelection;
	int		mCurrentSelection;
	int		mNbSelection;


	sf::SoundBuffer mNavigationBuffer;
	sf::SoundBuffer mValidationBuffer;

	sf::Sound mNavigation[3];
	sf::Sound mValidation[3];

	int mCurrentNavigation;
	int mCurrentValidation;

	sf::Music* mMusic;

	sf::String mAskedResolution;
	std::map<InputType, int> mMappingActual;

	bool		mWaitingForKey;
	InputType	mKeyWaited;
	float		mTimeSinceLastPushed;

public:
	OptionScreen(Player* pPlayer, sf::Music* pMusic);
	~OptionScreen(void);

	void update();
	void draw(sf::RenderTarget* pTarget);

	void moveSelector();
	void handleInput();

	void WaitForKey();
};

#endif
