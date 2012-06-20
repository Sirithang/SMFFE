#ifndef SMFFE_STAGESCREEN_H
#define SMFFE_STAGESCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "AScreen.h"
#include "Sprite.h"
#include "Player.h"

#include <vector>
#include <map>

class StageScreen : public AScreen
{
private:
	Player* mPlayer1;
	Player* mPlayer2;

	Sprite mBackground;

	sf::Text mArenaName;
	sf::Font mFont;

	sf::Music* mMusic;

	sf::SoundBuffer mNavigationBuffer;
	sf::SoundBuffer mValidationBuffer;

	sf::Sound mNavigation[3];
	sf::Sound mValidation[3];

	int mCurrentNavigation;
	int mCurrentValidation;

	std::map<int,sf::Vector2f> mPositions;
	std::vector<Sprite*>	mListSprites;
	std::vector<sf::String> mListNames;

	int mCurrentndex;
	int mPreviousIndex;
	int mNext;

	bool mTrainingMode;

	void setIndex(int pIndex);

public:
	StageScreen(Player* pPlayer1, Player* pPlayer2, sf::Music* pMusic = NULL, bool pTrainingMode = false);
	~StageScreen();

	void update();
	void draw(sf::RenderTarget* pTarget);
};

#endif