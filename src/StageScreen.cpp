#include "StageScreen.h"

#include "constants.h"

#include "SMFFEConfig.h"

#include "KeyboardInput.h"

#include "JoypadInput.h"

#include "LoadingScreen.h"

#include <dirent.h>

StageScreen::StageScreen(Player* pPlayer1, Player* pPlayer2, sf::Music* pMusic, bool pTrainingMode)
	:	mTrainingMode(pTrainingMode),
	mBackground("data/screens/StageScreen/Selection arene.png", sf::Vector2f(0,0))
{
	mPlayer1 = pPlayer1;
	mPlayer2 = pPlayer2;

	mFont.LoadFromFile("data/font/graph_35_pix.ttf");
	mArenaName.SetFont(mFont);
	mArenaName.SetPosition( 50 + SMFFEConfig::instance().getViewWidth() * - 0.5f,
							120 + SMFFEConfig::instance().getViewHeight() * -0.5f);

	mBackground.setPosition(mBackground.getSFSprite().GetSize() * - 0.5f);

	Sprite* lSprite;

	if(pMusic != NULL)
	{
		mMusic = pMusic;
	}
	else
	{
		mMusic = new sf::Music();
		mMusic->OpenFromFile("data/screens/startScreen/music.ogg");
		mMusic->SetLoop(true);
		mMusic->Play();
	}

	DIR *lDir;
	struct dirent *ent;

	lDir = opendir(gStageScreenPath.c_str());
	if (lDir != NULL) 
	{
	  /* print all the files and directories within directory */
	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_DIR)
		  {
			sf::String lStr(ent->d_name);

			if(lStr == "." || lStr == "..")
				continue;

			lSprite = new Sprite(gStageScreenPath+sf::String(ent->d_name)+"/vignette.png", sf::Vector2f(0.0f,0.0f));
			mListSprites.push_back(lSprite);
			mListNames.push_back(ent->d_name);
		  }
	  }
	  closedir (lDir);
	}

	
	mPositions[0] = sf::Vector2f(mListSprites[0]->getSFSprite().GetSize().x * -0.5f,
								  - 98);

	mPositions[1] =  sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.3f - mListSprites[0]->getSFSprite().GetSize().x * 0.5f * 0.5f,
									-98 + mListSprites[0]->getSFSprite().GetSize().y*0.5f);

	mPositions[2] =  sf::Vector2f(SMFFEConfig::instance().getViewWidth() *  0.3f - mListSprites[0]->getSFSprite().GetSize().x * 0.5f * 0.5f,
								 -98 + mListSprites[0]->getSFSprite().GetSize().y*0.5f);

	mNavigationBuffer.LoadFromFile("data/screens/SelectCharScreen/navigation.ogg");
	mValidationBuffer.LoadFromFile("data/screens/SelectCharScreen/validation.ogg");

	for(int i = 0; i < 3; i++)
	{
		mNavigation[i].SetBuffer(mNavigationBuffer);
		mValidation[i].SetBuffer(mValidationBuffer);
	}

	mCurrentNavigation = 0;
	mCurrentValidation = 0;

	setIndex(0);
}

StageScreen::~StageScreen()
{
}

//--------------------------------------

void StageScreen::setIndex(int pIndex)
{
	mCurrentndex = pIndex;
	if(mCurrentndex < 0)
		mCurrentndex = mListSprites.size() - 1;
	else if(mCurrentndex >= mListSprites.size())
		mCurrentndex = 0;

	if(mCurrentndex - 1 >= 0)
		mPreviousIndex = 0;
	else
		mPreviousIndex = mListSprites.size() - 1;

	if(mCurrentndex + 1 < mListSprites.size())
		mNext = mCurrentndex + 1;
	else
		mNext = 0;

	mNavigation[mCurrentNavigation].Play();
	mCurrentNavigation = (mCurrentNavigation + 1) % 3;

	mArenaName.SetString(mListNames[mCurrentndex]);
}

//------------------------------------------

void StageScreen::update()
{
	if(!SMFFEConfig::instance().mMusicOn)
	{
		mMusic->SetVolume(0);
	}
	else
	{
		mMusic->SetVolume(100);
	}

	//*****************

	mPlayer1->update();

	if(mPlayer1->getInput()->getInputDown(LEFT))
	{
		setIndex(mCurrentndex - 1);
	}
	else if(mPlayer1->getInput()->getInputDown(RIGHT))
	{
		setIndex(mCurrentndex + 1);
	}
	else if(mPlayer1->getInput()->getInputDown(C_LP))
	{
		delete mMusic;
		mValidation[0].Play();

		SMFFEConfig::instance().sArene =  mListNames[mCurrentndex];
		SMFFEConfig::instance().sTrainingMode = mTrainingMode;

		mManager->popScreen();
		mManager->pushScreen(new LoadingScreen(mPlayer1, mPlayer2));
	}
}

//------------------------------------------

void StageScreen::draw(sf::RenderTarget* pTarget)
{
	mBackground.draw(pTarget);

	pTarget->Draw(mArenaName);

	mListSprites[mCurrentndex]->getSFSprite().SetScale(1.0f, 1.0f);
	mListSprites[mCurrentndex]->setPosition(mPositions[0]);
	mListSprites[mCurrentndex]->draw(pTarget);

	mListSprites[mPreviousIndex]->getSFSprite().SetScale(0.5f, 0.5f);
	mListSprites[mPreviousIndex]->setPosition(mPositions[1]);
	mListSprites[mPreviousIndex]->draw(pTarget);

	mListSprites[mNext]->getSFSprite().SetScale(0.5f, 0.5f);
	mListSprites[mNext]->setPosition(mPositions[2]);
	mListSprites[mNext]->draw(pTarget);
}