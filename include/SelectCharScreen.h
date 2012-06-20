#ifndef SMFFE_SELECTCHARSCREEN_H
#define SMFFE_SELECTCHARSCREEN_H

#include <SFML/Graphics.hpp>

#include "AScreen.h"
#include "Sprite.h"
#include "Player.h"

#include <vector>
#include <map>


class SelectCharScreen : public AScreen
{
private:
	enum STATE
	{
		SELECT_CHAR,
		SELECT_ULTRA,
		SELECT_DONE
	};


	STATE mState1;
	STATE mState2;

	sf::Texture mBannerTexture;
	sf::Sprite  mBannerSprite;

	Sprite mScrollingText;

	Player* mPlayer1;
	Player* mPlayer2;

	std::vector<Sprite*>	mFrise1;
	std::vector<sf::String> mFrise1Name;

	std::vector<Sprite*>	mFrise2;
	std::vector<sf::String> mFrise2Name;

	Sprite*	mUltra1;
	Sprite*	mUltra2;

	Sprite*	mUltraSelect1;
	Sprite*	mUltraSelect2;

	Sprite	mPressLp;

	AnimatedSprite* mFond1;
	AnimatedSprite* mFond2;

	Sprite mBackground;
	Sprite mUpperBackground;

	std::map<int,sf::Vector2f> mPosition1;
	std::map<int,sf::Vector2f> mPosition2;

	std::vector<AnimatedSprite*> mListTextureFond;

	sf::String mPlayer1Select;
	sf::String mPlayer2Select;

	sf::Text mName1;
	sf::Text mName2;

	sf::Font mFont;

	sf::Music* mMusic;

	sf::SoundBuffer mNavigationBuffer;
	sf::SoundBuffer mValidationBuffer;

	sf::Sound mNavigation[3];
	sf::Sound mValidation[3];
	int mCurrentNavigation;
	int mCurrentValidation;

	bool mUltraCasu1;
	bool mUltraCasu2;

	int mNombreChar;
	int mCurrentPosition1, mPreviousPosition1, mNextPosition1;
	int mCurrentPosition2, mPreviousPosition2, mNextPosition2;

	bool mValidPerso1, mValidUltra1;
	bool mValidPerso2, mValidUltra2;
	bool mSelectUltraUp1, mSelectUltraUp2;

	bool	mTrainingMode;
	Player* mTemp;

public:
	SelectCharScreen(Player* pFirstPlayer, Player* pSecondPlayer, sf::Music* pMusic = NULL, bool pTraining = false);
	~SelectCharScreen(void);

	void selectChar();
	void selectUltra();
	void update();
	void draw(sf::RenderTarget* pTarger);

	void waitForInputP2();

};
#endif
