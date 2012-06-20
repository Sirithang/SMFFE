#ifndef SMFFE_COMBATSCREEN_H
#define SMFFE_COMBATSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "ascreen.h"
#include "Player.h"

class CombatScreen :
	public AScreen
{
protected:
	Player* mPlayer1;
	Player* mPlayer2;

	bool	mF5Released;
	bool	mTrainingMode;

	AnimatedSprite* mArene;
	float mRemainingTime;

	int state;
	float mTimer; //usefull to count time for states

	Sprite	mFight;
	Sprite	mReady;
	Sprite	mRoundOne;
	Sprite	mRoundTwo; 
	Sprite	mRoundFinal;

	Sprite  mPlayerOneRound;
	Sprite	mPlayerTwoRound;
	Sprite	mPlayerOneWin;
	Sprite	mPlayerTwoWin;

	sf::Font	mFont;
	sf::Text	mCountdownText;

	sf::Music	mMusic;
	float		mStartingVolume;


	sf::SoundBuffer	mRoundOneBuffer;
	sf::SoundBuffer	mRoundTwoBuffer;
	sf::SoundBuffer	mFinalRoundBuffer;
	sf::SoundBuffer mFightBuffer;
	sf::SoundBuffer	mVictoryBuffer[3];
	sf::SoundBuffer	mCheersBuffer;

	sf::Sound	mRoundOneSnd;
	sf::Sound	mRoundTwoSnd;
	sf::Sound	mFinalRoundSnd;
	sf::Sound	mVictory[3];
	sf::Sound	mCheers;
	sf::Sound	mFightSnd;
	int			mChoosenVictory;
	bool		mRoundPlayed;

	int mRound;

	sf::Vector2f mPositions[3];

	Sprite* mInfoDisplayed;

public:
	CombatScreen(Player* pPlayer1, Player* pPlayer2,  AnimatedSprite* pArene, bool pTrainingMode = false);
	~CombatScreen(void);

	void draw(sf::RenderTarget* pTarget);
	void update();
};

#endif
