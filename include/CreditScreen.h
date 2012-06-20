#ifndef SMFFE_CREDIT_SCREEN
#define SMFFE_CREDIT_SCREEN

#include "ascreen.h"

#include "Player.h"
#include "MenuScreen.h"

#include "Sprite.h"

class CreditScreen :
	public AScreen
{
protected:
	Player* mPlayer;

	Sprite mMorgane;
	Sprite mDavid;
	Sprite mMathieu;
	Sprite mAlexandra;
	Sprite mMichel;
	Sprite mSimon;
	Sprite mGuillaume;

	Sprite mBackground;

	sf::Font mFont;
	sf::Text mText;

	void setDefaultPos();

public:
	CreditScreen(Player* pPlayer);
	~CreditScreen(void);

	void update();
	void draw(sf::RenderTarget* ptarget);
};

#endif
