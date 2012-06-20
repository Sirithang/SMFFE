#include "..\include\LoadingScreen.h"

#include "SMFFEConfig.h"
#include "CombatScreen.h"

LoadingScreen::LoadingScreen(Player* pPlayer1, Player* pPlayer2)
	:mPlayer1(pPlayer1),
	mPlayer2(pPlayer2),
	mBackGround("data/screens/loadingScreen/Loading.png", sf::Vector2f(0,0)),
	mNyan("data/screens/loadingScreen/loading defilement.png", sf::Vector2f(0,0))
{
	mBackGround.setPosition(mBackGround.getSFSprite().GetSize() * - 0.5f);

	pPlayer1->setCharacter(SMFFEConfig::instance().sChar1, SMFFEConfig::instance().sCasu1);
	mPlayer2->setCharacter(SMFFEConfig::instance().sChar2, SMFFEConfig::instance().sCasu2);

	mPurcentage = 0;

	mBufferValidation.LoadFromFile("data/screens/MenuScreen/validation.ogg");
	mValidation.SetBuffer(mBufferValidation);

	mFont.LoadFromFile("data/font/graph_35_pix.ttf");

	mPressButton.SetColor(sf::Color::Black);
	mPressButton.SetFont(mFont);
	mPressButton.SetString("Press START to continue");
	mPressButton.SetPosition(	-mPressButton.GetRect().Width * 0.5f, 
								200);

	mEndLoading.LoadFromFile("data/sound/SF Hadoutsu.ogg");
	mLoading.SetBuffer(mEndLoading);

	mLoadEnded = false;
}


LoadingScreen::~LoadingScreen(void)
{

}

//****************************************

void LoadingScreen::update()
{
	mPurcentage = mPlayer1->getCharacter()->mPurcentageLoaded * 0.5f + mPlayer2->getCharacter()->mPurcentageLoaded * 0.5f;

	/*if(mPlayer1->getCharacter()->mPurcentageLoaded > 0.99f)
	{
		if(mPlayer2->getCharacter() == NULL)
		{
			mPlayer2->setCharacter(SMFFEConfig::instance().sChar2, SMFFEConfig::instance().sCasu2);
		}
		else
		{
			mPurcentage = 0.5f + mPlayer2->getCharacter()->mPurcentageLoaded * 0.5f;
			if(mPlayer2->getCharacter()->mPurcentageLoaded > 0.99f)
			{
				AnimatedSprite* lSpr = new AnimatedSprite(sf::Vector2f(0,0));
				lSpr->addAnimation("idle", sf::String("data/stages/")+SMFFEConfig::instance().sArene+"/anim", 99.0f);
				lSpr->play("idle");

				mManager->popScreen();
				mManager->pushScreen(new CombatScreen(mPlayer1, mPlayer2, lSpr, SMFFEConfig::instance().sTrainingMode));
			}
		}
	}*/

	mPlayer1->getInput()->pollData();

	if(mPlayer1->getCharacter()->mPurcentageLoaded > 0.99f && mPlayer2->getCharacter()->mPurcentageLoaded > 0.99f)
	{
		if(!mLoadEnded)
		{
			mLoadEnded = true;
			mLoading.Play();
		}

		if(mPlayer1->getInput()->getInputDown(START))
		{
			mValidation.Play();
			AnimatedSprite* lSpr = new AnimatedSprite(sf::Vector2f(0,0));
			lSpr->addAnimation("idle", sf::String("data/stages/")+SMFFEConfig::instance().sArene+"/anim", 99.0f);
			lSpr->play("idle");

			mManager->popScreen();
			mManager->pushScreen(new CombatScreen(mPlayer1, mPlayer2, lSpr, SMFFEConfig::instance().sTrainingMode));
		}
	}
}

//---------------------------------------

void LoadingScreen::draw(sf::RenderTarget* pTarget)
{
	mBackGround.draw(pTarget);

	mNyan.setPosition(sf::Vector2f(	SMFFEConfig::instance().getViewWidth() * -0.5f  - (1.0f - mPurcentage) * mNyan.getSFSprite().GetSize().x, 
									SMFFEConfig::instance().getViewHeight() * -0.5f));

	mNyan.draw(pTarget);

	if(mPurcentage > 0.99f)
	{
		pTarget->Draw(mPressButton);
	}
}