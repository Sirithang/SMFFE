#include "..\include\Player.h"

#include "SMFFEConfig.h"

Player::Player(CommandInput* pInput, bool pLeftSide)
	: mLeftSide(pLeftSide)
{
	mInput = pInput;
	mChar = NULL;
	mUpdatePlayer = false;
}


Player::~Player(void)
{
}

//------------------------------------

void Player::update()
{
	if(mUpdatePlayer && mChar != NULL)
		mChar->update();

	mInput->pollData();
}

//------------------------------------

void Player::draw(sf::RenderTarget* pTarget)
{
	if(mUpdatePlayer && mChar != NULL)
		mChar->draw(pTarget);
}

//------------------------------------

void Player::setCharacter(sf::String pChar, bool pCasu)
{
	sf::Vector2f lPos;

	if(mLeftSide)
		lPos = sf::Vector2f(100.0f,0.0f);
	else
		lPos = sf::Vector2f(SMFFEConfig::instance().getViewWidth(), 0.0);

	mChar = new Character(lPos, this, pCasu);

	mCharpath = sf::String("data/persos/") + pChar;
	//mChar->getSFSprite().SetScale(3.0f, 3.0f);//TO REMOVE DEBUG PURPOSE
	
	lThread = new sf::Thread(&Player::loadChar, this);
	lThread->Launch();
}

//--------------------------------------

void Player::deleteChar()
{
	delete mChar;
	mChar = NULL;
}