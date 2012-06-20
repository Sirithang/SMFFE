#ifndef SMFFE_STARTSCREEN_H
#define SMFFE_STARTSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "AScreen.h"

#include <vector>


class StartScreen : public AScreen
{
private:
	sf::Texture mBackground;
	sf::Sprite	mBackgroundSpr;

	sf::Texture mBannerTexture;
	sf::Sprite  mBannerSprite;

	sf::Texture mStartTexture;
	sf::Sprite  mStartSprite;

	sf::SoundBuffer	mBuffer;
	sf::Sound	mSound;

	sf::Music*	mMusique;

	int mMulti;

	std::vector<int> mJoypads;

public:
	StartScreen(void);
	~StartScreen(void);

	void update();
	void draw(sf::RenderTarget* pTarget);
};

#endif
