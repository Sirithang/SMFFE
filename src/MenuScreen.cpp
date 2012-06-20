#include "..\include\MenuScreen.h"

#include "SMFFEConfig.h"
#include "SelectCharScreen.h"
#include "OptionScreen.h"
#include "CreditScreen.h"

MenuScreen::MenuScreen(CommandInput* pInput, sf::Music* pMusic)
	:mBanner("data/screens/MenuScreen/menu.png", sf::Vector2f(0, 0)),
	mVersusNormal("data/screens/MenuScreen/versus mode.png", sf::Vector2f(0, 0)),
	mVersusHighlight("data/screens/MenuScreen/surbrillance/versus mode surbrillance.png", sf::Vector2f(0, 0)), 
	mTrainingNormal("data/screens/MenuScreen/training.png", sf::Vector2f(0, 0)), 
	mTrainingHighlight("data/screens/MenuScreen/surbrillance/training surbrillance.png", sf::Vector2f(0, 0)), 
	mOptionNormal("data/screens/MenuScreen/options.png", sf::Vector2f(0, 0)), 
	mOptionHighlight("data/screens/MenuScreen/surbrillance/options surbrillance.png", sf::Vector2f(0, 0)), 
	mCreditNormal("data/screens/MenuScreen/credits.png", sf::Vector2f(0, 0)), 
	mCreditHighlight("data/screens/MenuScreen/surbrillance/credits surbrillance.png", sf::Vector2f(0, 0)),
	mBackground("data/GUI/menu background.png", sf::Vector2f(0,0))
{
	mBackground.setPosition(mBackground.getSFSprite().GetSize() * -0.5f);
	mBanner.setPosition(sf::Vector2f(	- mBanner.getSFSprite().GetSize().x * 0.5f,
										- SMFFEConfig::instance().getViewHeight() * 0.5f));

	mVersusNormal.setPosition(sf::Vector2f(	-mVersusNormal.getSFSprite().GetSize().x * 0.5f,
										0));
	mVersusHighlight.setPosition(sf::Vector2f(-mVersusHighlight.getSFSprite().GetSize().x * 0.5f,
										0));

	mTrainingNormal.setPosition(sf::Vector2f(  -mTrainingNormal.getSFSprite().GetSize().x * 0.5f,
		mVersusNormal.getPosition().y + mVersusNormal.getSFSprite().GetSize().y));
	mTrainingHighlight.setPosition(sf::Vector2f(  -mTrainingHighlight.getSFSprite().GetSize().x * 0.5f,
													mVersusNormal.getPosition().y + mVersusNormal.getSFSprite().GetSize().y));

	mOptionNormal.setPosition(sf::Vector2f(  -mOptionNormal.getSFSprite().GetSize().x * 0.5f,
												mTrainingNormal.getPosition().y + mTrainingNormal.getSFSprite().GetSize().y));
	mOptionHighlight.setPosition(sf::Vector2f(  -mOptionHighlight.getSFSprite().GetSize().x * 0.5f,
													mTrainingNormal.getPosition().y + mTrainingNormal.getSFSprite().GetSize().y));

	mCreditNormal.setPosition(sf::Vector2f(  -mCreditNormal.getSFSprite().GetSize().x * 0.5f,
												mOptionNormal.getPosition().y + mOptionNormal.getSFSprite().GetSize().y));
	mCreditHighlight.setPosition(sf::Vector2f(  -mCreditHighlight.getSFSprite().GetSize().x * 0.5f,
													mOptionNormal.getPosition().y + mOptionNormal.getSFSprite().GetSize().y));

	mVersus = &mVersusHighlight;
	mTraining = &mTrainingNormal;
	mOption = &mOptionNormal;
	mCredit = &mCreditNormal;

	mFirstPlayer = new Player(pInput, true);

	mNbChoice = 4;
	mCurrentChoice = 0;

	if(pMusic != NULL)
		mMusic = pMusic;
	else
	{
		mMusic = new sf::Music;
		mMusic->OpenFromFile("data/screens/startScreen/music.ogg");
		mMusic->SetLoop(true);
		mMusic->Play();
	}

	

	moveSelector();

	mBufferNavigation.LoadFromFile("data/screens/MenuScreen/navigation.ogg");
	mBufferValidation.LoadFromFile("data/screens/MenuScreen/validation.ogg");

	for(int i = 0; i < 3; i++)
		mNavigation[i].SetBuffer(mBufferNavigation);

	mValidation.SetBuffer(mBufferValidation);

	mCurrentNavigation = 0;
}


MenuScreen::~MenuScreen(void)
{
}

//--------------------------------------------------------------------------

void MenuScreen::draw(sf::RenderTarget* pTarget)
{
	//pTarget->Clear(sf::Color::Black);

	mBackground.draw(pTarget);
	mBanner.draw(pTarget);

	mVersus->draw(pTarget);
	mTraining->draw(pTarget);
	mOption->draw(pTarget);
	mCredit->draw(pTarget);
}

//---------------------------------------------------------------------------

void MenuScreen::update()
{
	if(!SMFFEConfig::instance().mMusicOn)
	{
		mMusic->SetVolume(0);
	}
	else
	{
		mMusic->SetVolume(100);
	}

	mFirstPlayer->update();

	if(mFirstPlayer->getInput()->getInputDown(BACK))
	{
		mManager->getWin()->Close();
		return;
	}

	if(mFirstPlayer->getInput()->getInputDown(DOWN))
	{
		mCurrentChoice += 1;
		mNavigation[mCurrentNavigation].Play();
		mCurrentNavigation = (mCurrentNavigation+1)%3;
	}
	else if(mFirstPlayer->getInput()->getInputDown(UP))
	{
		mCurrentChoice -= 1;
		mNavigation[mCurrentNavigation].Play();
		mCurrentNavigation = (mCurrentNavigation+1)%3;
	}
	else if(mFirstPlayer->getInput()->getInputDown(C_LP))
	{
		gotoChoice();
	}

	if(mCurrentChoice <0)
		mCurrentChoice += mNbChoice;
	else if(mCurrentChoice >= mNbChoice)
		mCurrentChoice -= mNbChoice;

	moveSelector();
}

//----------------------------------------------------------------------------

void MenuScreen::moveSelector()
{
	switch(mCurrentChoice)
	{
	case 0:
		mVersus = &mVersusHighlight;
		mTraining = &mTrainingNormal;
		mOption = &mOptionNormal;
		mCredit = &mCreditNormal;
		break;
	case 1:
		mVersus = &mVersusNormal;
		mTraining = &mTrainingHighlight;
		mOption = &mOptionNormal;
		mCredit = &mCreditNormal;
		break;
	case 2:
		mVersus = &mVersusNormal;
		mTraining = &mTrainingNormal;
		mOption = &mOptionHighlight;
		mCredit = &mCreditNormal;
		break;
	case 3:
		mVersus = &mVersusNormal;
		mTraining = &mTrainingNormal;
		mOption = &mOptionNormal;
		mCredit = &mCreditHighlight;
		break;
	default:
		break;
	}
}

//-------------------------------------------------------

void MenuScreen::gotoChoice()
{
	mIsFrozen = false;
	switch(mCurrentChoice)
	{
	case 0:
		mValidation.Play();
		mManager->popScreen();
		mManager->pushScreen(new SelectCharScreen(mFirstPlayer, NULL, mMusic));
		break;
	case 1:
		mValidation.Play();
		mManager->popScreen();
		mManager->pushScreen(new SelectCharScreen(mFirstPlayer, NULL, mMusic, true));
		break;
	case 2:
		mValidation.Play();
		mManager->pushScreen(new OptionScreen(mFirstPlayer, mMusic));
		break;
	case 3:
		mValidation.Play();
		//mManager->popScreen();
		mManager->pushScreen(new CreditScreen(mFirstPlayer));
		break;
	default:
		return;
		break;
	}
}