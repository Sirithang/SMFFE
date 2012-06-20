#include "..\include\CombatScreen.h"

#include "JoypadInput.h"
#include "KeyboardInput.h"

#include "constants.h"
#include "Helpers.h"

#include "SelectCharScreen.h"
#include "OptionScreen.h"

#include "TimedSprite.h"
#include "SMFFEConfig.h"

#include "SMFFETime.h"

CombatScreen::CombatScreen(Player* pPlayer1, Player* pPlayer2, AnimatedSprite* pArene, bool pTrainingMode)
	:	mReady("data/GUI/Messages/Ready.png", sf::Vector2f(0,0)),
		mFight("data/GUI/Messages/Fight.png", sf::Vector2f(0,0)),
		mRoundOne("data/GUI/Messages/Roundone.png", sf::Vector2f(0,0)),
		mRoundTwo("data/GUI/Messages/Roundtwo.png", sf::Vector2f(0,0)),
		mRoundFinal("data/GUI/Messages/Finalround.png", sf::Vector2f(0,0)),
		mPlayerOneRound("data/GUI/Messages/Playeronewins.png", sf::Vector2f(0,0)),
		mPlayerTwoRound("data/GUI/Messages/Playertwowins.png", sf::Vector2f(0,0)),
		mPlayerOneWin("data/GUI/Messages/Playeronevictory.png", sf::Vector2f(0,0)),
		mPlayerTwoWin("data/GUI/Messages/Playertwovictory.png", sf::Vector2f(0,0)),
		mTrainingMode(pTrainingMode)
{
	//mPlayer1 = new Player(new JoypadInput(0), false);
	//mPlayer2 = new Player(new KeyboardInput(), true);
	mPlayer1 = pPlayer1;
	mPlayer2 = pPlayer2;

	mRoundPlayed = false;

	if(mPlayer1 != NULL)
		mPlayer1->setUpdatePlayer(true);
	if(mPlayer2 != NULL)
		mPlayer2->setUpdatePlayer(true);

	mPlayer1->getCharacter()->setOtherCharacter(mPlayer2->getCharacter());
	mPlayer2->getCharacter()->setOtherCharacter(mPlayer1->getCharacter());

	//mPlayer2->getCharacter()->setPosition(sf::Vector2f(700.0f, 0.0f));

	mF5Released = true;

	mView = sf::View(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.5f, SMFFEConfig::instance().getViewHeight() * 0.5f), sf::Vector2f(SMFFEConfig::instance().getViewWidth(), SMFFEConfig::instance().getViewHeight()));

	//****** STAGE LOADING *******

	mArene = pArene;
	//mArene->addAnimation("idle", sf::String("data/stages/")+pArene+"/anim", 99.0f);
	//mArene->play("idle");

	mMusic.OpenFromFile(sf::String("data/stages/")+SMFFEConfig::instance().sArene+"/theme.ogg");
	mMusic.SetLoop(true);
	mStartingVolume = 50.0f;

	//***********Sound Loading********

	mRoundOneBuffer.LoadFromFile("data/sound/Rounds/MK Round One.ogg");
	mRoundTwoBuffer.LoadFromFile("data/sound/Rounds/MK Round Two.ogg");
	mFinalRoundBuffer.LoadFromFile("data/sound/Rounds/MK Final Round.ogg");

	mVictoryBuffer[0].LoadFromFile("data/sound/Rounds/MK Excellent.ogg");
	mVictoryBuffer[1].LoadFromFile("data/sound/Rounds/MK Outstanding.ogg");
	mVictoryBuffer[2].LoadFromFile("data/sound/Rounds/MK Well done.ogg");

	mCheersBuffer.LoadFromFile("data/sound/Rounds/MK Crowd Cheering.ogg");

	mFightBuffer.LoadFromFile("data/sound/Rounds/MK Fight.ogg");

	mRoundOneSnd.SetBuffer(mRoundOneBuffer);
	mRoundTwoSnd.SetBuffer(mRoundTwoBuffer);
	mFinalRoundSnd.SetBuffer(mFinalRoundBuffer);
	
	mVictory[0].SetBuffer(mVictoryBuffer[0]);
	mVictory[1].SetBuffer(mVictoryBuffer[1]);
	mVictory[2].SetBuffer(mVictoryBuffer[2]);

	mFightSnd.SetBuffer(mFightBuffer);

	mCheers.SetBuffer(mCheersBuffer);

	//********************************

	state = 0;

	mRemainingTime = 99;
	mTimer = 0;

	mPlayer1->getCharacter()->setCanControl(false);
	mPlayer2->getCharacter()->setCanControl(false);

	mInfoDisplayed = &mRoundOne;
	mRound = 1;

	mFont.LoadFromFile("data/font/ka1.ttf");
	mCountdownText.SetString("99");
	mCountdownText.SetColor(sf::Color::Black);
	mCountdownText.SetFont(mFont);
	mCountdownText.SetCharacterSize(40);
	mCountdownText.SetPosition(	SMFFEConfig::instance().getViewWidth() * 0.5f - mCountdownText.GetRect().Width * 0.5f,
								30);

	if(mTrainingMode)
	{
		mPlayer2->getCharacter()->setTrainingMode(true);
		mPlayer2->getCharacter()->setPosition(sf::Vector2f(600, 100));
		mPlayer1->getCharacter()->setTrainingMode(true);
	}
}


CombatScreen::~CombatScreen(void)
{
	mPlayer1->deleteChar();
	mPlayer2->deleteChar();
	delete mArene;
	mMusic.Stop();
}

//----------------------------------------

void CombatScreen::draw(sf::RenderTarget* pTarget)
{
	pTarget->Clear(sf::Color::White);

	mArene->draw(pTarget);

	pTarget->Draw(mCountdownText);

	mPlayer1->draw(pTarget);
	mPlayer2->draw(pTarget);

	std::vector<Sprite*> lSprs = mPlayer1->getCharacter()->getAssociatedSprite();
	for(int i = 0; i < lSprs.size(); i++)
	{
		lSprs.at(i)->draw(pTarget);
	}

	lSprs = mPlayer2->getCharacter()->getAssociatedSprite();
	for(int i = 0; i < lSprs.size(); i++)
	{
		lSprs.at(i)->draw(pTarget);
	}

	if(mInfoDisplayed != NULL)
		mInfoDisplayed->draw(pTarget);
}

//-----------------------------------------

void CombatScreen::update()
{
	if(!SMFFEConfig::instance().mMusicOn)
	{
		mMusic.SetVolume(0);
	}
	/*else
	{
		mMusic.SetVolume(mStartingVolume);
	}*/

	mTimer += Time::deltaTime();

	mPlayer1->getCharacter()->update();
	mPlayer2->getCharacter()->update();

	//****************************************************

	if(mPlayer1->getInput()->getInputDown(START))
	{
		mIsFrozen = true;
		mManager->pushScreen(new OptionScreen(mPlayer1, &mMusic));
	}
	else if(mPlayer2->getInput()->getInputDown(START))
	{
		mIsFrozen = true;
		mManager->pushScreen(new OptionScreen(mPlayer2, &mMusic));
	}


	//****************************************************


	if((mPlayer1->getCharacter()->isInUltra() || mPlayer2->getCharacter()->isInUltra())
		&& mMusic.GetVolume() > 0)
	{
		mMusic.SetVolume(std::max(mMusic.GetVolume() - Time::deltaTime() * 20, 0.0f));
	}
	else if(mMusic.GetVolume() < mStartingVolume)
	{
		mMusic.SetVolume(std::min(mMusic.GetVolume() + Time::deltaTime() * 20, mStartingVolume));
	}

	mArene->update();

	if(mTrainingMode && mPlayer1->getInput()->getInputDown(BACK))
	{
		mMusic.Stop();
		mManager->popScreen();
		mManager->pushScreen(new SelectCharScreen(mPlayer1, NULL, NULL, true));
		return;
	}

	if(!mTrainingMode)
		mCountdownText.SetString(intToString(ceil(mRemainingTime)));
	else
		mCountdownText.SetString("xx");

	if(state == 0)
	{//Round
		mPlayer1->getCharacter()->play("idle");
		mPlayer2->getCharacter()->play("idle");
		
		if(!mRoundPlayed)
		{
			switch(mRound)
			{
			case 1:
				mRoundOneSnd.Play();
				break;
			case 2:
				mRoundTwoSnd.Play();
				break;
			case 3:
				mFinalRoundSnd.Play();
				break;
			default:
				break;
			}
			mRoundPlayed = true;
		}

		if(mTimer > 2.0f)
		{
			mMusic.SetVolume(mStartingVolume);
			mMusic.Play();

			for(int i = 0; i < 3; i++)
				mVictory[i].Stop();

			mCheers.Stop();

			mTimer = 0;
			mInfoDisplayed = &mReady;

			state = 1;
		}
	}
	else if(state == 1)
	{//ready
		if(mTimer > 2.0f)
		{
			mPlayer1->getCharacter()->setCanControl(true);
			mPlayer2->getCharacter()->setCanControl(true);

			mTimer = 0;
			mInfoDisplayed = &mFight;

			mFightSnd.Play();

			state = 2;
		}
	}
	else if(state == 2)
	{//fight
		if(mTimer >= 2.0f)
		{
			mInfoDisplayed = NULL;
		}

		mArene->update();

		if(!mTrainingMode)
			mRemainingTime -= Time::deltaTime();

		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::F5) && mF5Released)
		{
			mPlayer1->getCharacter()->reloadInfo();
			mPlayer2->getCharacter()->reloadInfo();
			mF5Released = false;
		}
		else
			mF5Released = true;
		
		mPlayer1->getCharacter()->testMaskAgainst(mPlayer2->getCharacter(), true);

		mPlayer1->getCharacter()->lateUpdate();
		mPlayer2->getCharacter()->lateUpdate();

		mPlayer1->getCharacter()->checkCollisionsWithProjectiles(mPlayer2->getCharacter()->getProjectiles());
		mPlayer2->getCharacter()->checkCollisionsWithProjectiles(mPlayer1->getCharacter()->getProjectiles());

		std::vector<Sprite*> lSprs = mPlayer1->getCharacter()->getAssociatedSprite();
		std::vector<Sprite*>::iterator lIt = lSprs.begin();
		while(lIt != lSprs.end())
		{
			TimedSprite* lPt = static_cast<TimedSprite*>(*lIt);

			if(lPt->getRemainingTime() > 0)
				lIt++;
			else
			{
				delete *lIt;
				lIt = lSprs.erase(lIt);
			}
		}

		lSprs = mPlayer2->getCharacter()->getAssociatedSprite();
		lIt = lSprs.begin();
		while(lIt != lSprs.end())
		{
			TimedSprite* lPt = static_cast<TimedSprite*>(*lIt);

			if(lPt->getRemainingTime() > 0)
				lIt++;
			else
			{
				delete *lIt;
				lIt = lSprs.erase(lIt);
			}
		}

		if(mRemainingTime <= 0)
		{
			mRemainingTime = 0;
			
			
			if(mPlayer1->getCharacter()->getLifePurcentage() < mPlayer2->getCharacter()->getLifePurcentage())
			{
				mPlayer1->getCharacter()->setCanControl(false);
				mPlayer2->getCharacter()->setCanControl(false);

				mPlayer1->getCharacter()->die();
				mPlayer2->getCharacter()->winARound();

				if(mPlayer2->getCharacter()->getNumberOfRoundWon() == 1)
				{
					mInfoDisplayed = &mPlayerTwoRound;
					state = 3;
				}
				else if(mPlayer2->getCharacter()->getNumberOfRoundWon() == 2)
				{
					mInfoDisplayed = &mPlayerTwoWin;
					state = 4;
				}

				mTimer = 0;

			}
			else
			{
				
				mPlayer1->getCharacter()->setCanControl(false);
				mPlayer2->getCharacter()->setCanControl(false);

				mPlayer2->getCharacter()->die();
				mPlayer1->getCharacter()->winARound();

				if(mPlayer1->getCharacter()->getNumberOfRoundWon() == 1)
				{
					mInfoDisplayed = &mPlayerOneRound;
					state = 3;
				}
				else if(mPlayer1->getCharacter()->getNumberOfRoundWon() == 2)
				{
					mInfoDisplayed = &mPlayerOneWin;
					state = 4;
				}

				mTimer = 0;
			}
		}


		if(mPlayer1->getCharacter()->isDead() && !mPlayer2->getCharacter()->isInUltra())
		{
			mPlayer1->getCharacter()->setCanControl(false);
			mPlayer2->getCharacter()->setCanControl(false);

			mPlayer1->getCharacter()->die();
			mPlayer2->getCharacter()->winARound();

			if(mPlayer2->getCharacter()->getNumberOfRoundWon() == 1)
			{
				mInfoDisplayed = &mPlayerTwoRound;
				state = 3;
			}
			else if(mPlayer2->getCharacter()->getNumberOfRoundWon() == 2)
			{
				mInfoDisplayed = &mPlayerTwoWin;
				state = 4;
			}

			mTimer = 0;
		}
		else if(mPlayer2->getCharacter()->isDead() && !mPlayer1->getCharacter()->isInUltra())
		{
			mPlayer1->getCharacter()->setCanControl(false);
			mPlayer2->getCharacter()->setCanControl(false);

			mPlayer2->getCharacter()->die();
			mPlayer1->getCharacter()->winARound();

			if(mPlayer1->getCharacter()->getNumberOfRoundWon() == 1)
			{
				mInfoDisplayed = &mPlayerOneRound;
				state = 3;
			}
			else if(mPlayer1->getCharacter()->getNumberOfRoundWon() == 2)
			{
				mInfoDisplayed = &mPlayerOneWin;
				state = 4;
			}

			mTimer = 0;
		}
	}
	else if(state == 3)
	{
		mRoundPlayed = false;
		mMusic.SetVolume((1.0f - (mTimer / 4.0f)) * mStartingVolume);

		if(mCheers.GetStatus() == sf::Sound::Stopped)
		{
			mCheers.Play();
			mChoosenVictory = rand()%3;
		}

		if(mTimer > 4.0f)
		{
			mMusic.Stop();

			if(mVictory[mChoosenVictory].GetStatus() == sf::Sound::Stopped)
				mVictory[mChoosenVictory].Play();
		}

		if(mTimer > 7.0f)
		{
			mPlayer1->getCharacter()->reset();
			mPlayer2->getCharacter()->reset();
			state = 0;
			mTimer = 0;

			mRound++;

			mRemainingTime = 99;

			if(mRound == 2)
			{
				mInfoDisplayed = &mRoundTwo;
			}
			else if(mRound == 3)
			{
				mInfoDisplayed = &mRoundFinal;
			}

			mPlayer1->getCharacter()->setCanControl(false);
			mPlayer2->getCharacter()->setCanControl(false);
		}

	}
	else if( state == 4)
	{//exit after a wait
		if(mTimer > 4.0f)
		{
			mMusic.Stop();
			mManager->popScreen();
			mManager->pushScreen(new SelectCharScreen(mPlayer1, mPlayer2));
		}
	}

}