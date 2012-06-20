#include "SelectCharScreen.h"

#include "SMFFETime.h"

#include "constants.h"

#include "MenuScreen.h"
#include "StageScreen.h"

#include "SMFFEConfig.h"

#include "KeyboardInput.h"
#include "JoypadInput.h"
#include "IAInput.h"

#include <dirent.h>

SelectCharScreen::SelectCharScreen(Player* pFirstPlayer, Player* pSecondPlayer, sf::Music* pMusic, bool pTraining)
	: mBackground("data/GUI/MenuSelect.png", sf::Vector2f(0,0)),
	  mUpperBackground("data/GUI/Selection personnage 1024.png", sf::Vector2f(0,0)),
	  mScrollingText("data/GUI/Ecran de defilement.png", sf::Vector2f(0,0)),
	  mPressLp("data/GUI/starting_LP.png", sf::Vector2f(0,0)),
	  mTemp(NULL)
{
	mBackground.setPosition(mBackground.getSFSprite().GetSize() * -0.5f);
	mUpperBackground.setPosition(mUpperBackground.getSFSprite().GetSize() * -0.5f);
	mScrollingText.setPosition(sf::Vector2f(-512, -384) );

	mPlayer1 = pFirstPlayer;

	mFont.LoadFromFile("data/font/graph_35_pix.ttf");
	mName1.SetFont(mFont);
	mName2.SetFont(mFont);

	mName1.SetPosition(-360, 30);
	mName2.SetPosition(  50, 30);

	mPressLp.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f - mPressLp.getSFSprite().GetSize().x * 0.5f,
						 -270.0f));
		
	mTrainingMode = pTraining;

	if(mTrainingMode)
	{
		mPlayer2 = new Player(new IAInput(), false);
	}
	else
	{
		mPlayer2 = pSecondPlayer;//new Player(new JoypadInput(0), false);
	}

	mCurrentPosition1 = 1;
	mCurrentPosition2 = 1;
	mPreviousPosition1 = 0;
	mPreviousPosition2 = 0;
	mNextPosition1 = 2;
	mNextPosition2 = 2;

	mValidPerso1 = false;
	mValidPerso2 = false;
	mValidUltra1 = false;
	mValidUltra2 = false;
	mSelectUltraUp1 = true;
	mSelectUltraUp2 = true;

	Sprite* lSprite;
	sf::Texture* lTexture;
	sf::Color lColor;
	sf::Vector2f lPosition = sf::Vector2f(0.0f,0.0f);

	mNombreChar = 3;
	float lIndex = 0.0f;
	
	//******** Find all the perso defined

	DIR *lDir;
	struct dirent *ent;

	lDir = opendir(gSelectPersoPath.c_str());
	if (lDir != NULL) 
	{
	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_DIR)
		  {
			  sf::String lStr(ent->d_name);

			if(lStr == "." || lStr == "..")
				continue;

			sf::String lPort = gSelectPersoPath+sf::String(ent->d_name)+"/selectScreen/portrait.png";

			lSprite = new Sprite(lPort, lPosition);

			mFrise1.push_back(lSprite);
			mFrise2.push_back(lSprite);

			mFrise1Name.push_back(ent->d_name);
			mFrise2Name.push_back(ent->d_name);

			AnimatedSprite* lSprite = new AnimatedSprite(sf::Vector2f(0,0));
			lSprite->addAnimation("idle", gSelectPersoPath+sf::String(ent->d_name)+"/selectScreen/fond", 3.0f);
			lSprite->play("idle");
			mListTextureFond.push_back(lSprite);
		  }
	  }
	  closedir (lDir);
	}

	//************************************

	/*lDir = opendir(gSelectPersoFondPath.c_str());
	if (lDir != NULL) 
	{

	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_REG)
		  {
			  sf::String lStr(ent->d_name);
			  if(lStr.Find(".png") != sf::String::InvalidPos)
			  {
				  lTexture = new sf::Texture();
				  lTexture->LoadFromFile(gSelectPersoFondPath+sf::String(ent->d_name));
				  mListTextureFond.push_back(lTexture);
			  }
		  }
	  }
	  closedir (lDir);
	}*/

	//****** Size checking *******

	while(mFrise1.size() < 3)
	{
		mFrise1.push_back(mFrise1[0]);
		mFrise1Name.push_back(mFrise1Name[0]);
		mFrise2.push_back(mFrise2[0]);
		mFrise2Name.push_back(mFrise2Name[0]);

		mListTextureFond.push_back(mListTextureFond[0]);
	}

	/*if(mFrise1.size() < 3)
	{
		mFrise1.push_back(mFrise1[0]);
		mFrise1.push_back(mFrise1[1]);

		mListTextureFond.push_back(mListTextureFond[0]);
		mListTextureFond.push_back(mListTextureFond[1]);
	}

	if(mFrise2.size() < 3)
	{
		mFrise2.push_back(mFrise2[0]);
		mFrise2.push_back(mFrise2[1]);
	}*/

	//*********************************
	mNombreChar = 3;

	for(int i=0; i < mNombreChar; i++)
	{
		if(i == 0)
		{
			mPosition1[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.485f, SMFFEConfig::instance().getViewHeight() * 0.205f);
		}
		else if(i==1)
		{
			mPosition1[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.353f, SMFFEConfig::instance().getViewHeight() * 0.128f);
		}
		else
		{
			mPosition1[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.153f, SMFFEConfig::instance().getViewHeight() * 0.205f);
		}

		//lIndex += mFrise1[i]->getSFSprite().GetSize().x * lFactor + 10;
		mFrise1[i]->setPosition(mPosition1[i]);
	}

	lIndex = 0.0f;

	for(int i=0; i < mNombreChar; i++)
	{
		if(i == 0)
		{
			mPosition2[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.153f - mFrise2[0]->getSFSprite().GetSize().x * 0.6, SMFFEConfig::instance().getViewHeight() * 0.205f);
		}
		else if(i==1)
		{
			mPosition2[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.353f - mFrise2[0]->getSFSprite().GetSize().x, SMFFEConfig::instance().getViewHeight() * 0.128f);
		}
		else
		{
			mPosition2[i] = sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.485f - mFrise2[0]->getSFSprite().GetSize().x  * 0.6, SMFFEConfig::instance().getViewHeight() * 0.205f);
		}
	}
	
	mUltra1 = new Sprite("data/screens/SelectCharScreen/Ultra.png",sf::Vector2f(0.0f,0.0f));
	mUltra1->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), (mUltra1->getSFSprite().GetSize().y * -0.88f)));

	mUltraSelect1 = new Sprite("data/screens/SelectCharScreen/Select.png",sf::Vector2f(0.0f,0.0f));
	mUltraSelect1->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), (mUltra1->getSFSprite().GetSize().y *  -0.5f)));

	mUltra2 = new Sprite("data/screens/SelectCharScreen/Ultra.png",sf::Vector2f(0.0f,0.0f));
	mUltra2->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), (mUltra1->getSFSprite().GetSize().y *  -0.88f)));

	mUltraSelect2 = new Sprite("data/screens/SelectCharScreen/Select.png",sf::Vector2f(0.0f,0.0f));
	mUltraSelect2->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), (mUltra1->getSFSprite().GetSize().y *  -0.5)));

	mFond1 = NULL;
	mFond2 = NULL;

	mState1 = SELECT_CHAR;
	mState2 = SELECT_CHAR;

	mUltraCasu1 = false;
	mUltraCasu2 = false;

	mFond1 = mListTextureFond[mCurrentPosition1];
	mFond2 = mListTextureFond[mCurrentPosition2];

	mName1.SetString(mFrise1Name[mCurrentPosition1]);
	mName1.SetPosition(SMFFEConfig::instance().getViewWidth() * -0.25f - mName1.GetRect().Width *0.5f, 30);
	mName2.SetString(mFrise1Name[mCurrentPosition2]);
	mName2.SetPosition(SMFFEConfig::instance().getViewWidth() * 0.25f - mName2.GetRect().Width *0.5f, 30);

	mNavigationBuffer.LoadFromFile("data/screens/SelectCharScreen/navigation.ogg");
	mValidationBuffer.LoadFromFile("data/screens/SelectCharScreen/validation.ogg");

	for(int i = 0; i < 3; i++)
	{
		mNavigation[i].SetBuffer(mNavigationBuffer);
		mValidation[i].SetBuffer(mValidationBuffer);
	}

	mCurrentNavigation = 0;
	mCurrentValidation = 0;

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
}

SelectCharScreen::~SelectCharScreen(void)
{
	/*try
	{
		for(int i = 0; i < mFrise1.size(); i++)
			delete mFrise1[i];
	
		mFrise1.clear();

		for(int i = 0; i < mFrise2.size(); i++)
			delete mFrise2[i];

		mFrise2.clear();

		for(int i = 0; i < mListTextureFond.size(); i++)
			delete mListTextureFond[i];

		mListTextureFond.clear();
	}
	catch(std::exception e)
	{

	}*/
}

//--------------------------------------------
void SelectCharScreen::update()
{
	if(!SMFFEConfig::instance().mMusicOn)
	{
		mMusic->SetVolume(0);
	}
	else
	{
		mMusic->SetVolume(100);
	}

	//--------

	mFond1->update();

	mScrollingText.setPosition(mScrollingText.getPosition() - sf::Vector2f(Time::deltaTime() * 50, 0));

	if(mScrollingText.getPosition().x  <= -(1024 + 512))
	{
		mScrollingText.setPosition(sf::Vector2f(-512, -384));
	}


	if(mFond1 != mFond2)
		mFond2->update();

	if(mPlayer1->getInput()->getInputDown(C_HK) || mPlayer1->getInput()->getInputDown(BACK))
	{
		if(mState1 == SELECT_CHAR)
		{//go back to menu if at char select stage
			mManager->popScreen();
			mManager->pushScreen(new MenuScreen(mPlayer1->getInput(), mMusic));
		}
		else if(mState1 == SELECT_ULTRA)
		{
			mState1 = SELECT_CHAR;
		}
		else if(mState1 == SELECT_DONE)
		{
			mState1 = SELECT_ULTRA;
		}
	}

	if(mPlayer2 != NULL && (mPlayer2->getInput()->getInputDown(C_HK) || mPlayer2->getInput()->getInputDown(BACK)))
	{
		if(mTrainingMode && mState2 == SELECT_CHAR)
		{
			mState1 = SELECT_ULTRA;
			mPlayer1 = mPlayer2;
			mPlayer2 = mTemp;
		}

		if(mState2 == SELECT_ULTRA)
		{
			mState2 = SELECT_CHAR;
		}
		else if(mState2 == SELECT_DONE)
		{
			mState2 = SELECT_ULTRA;
		}
	}

	//***************

	selectUltra();
	selectChar();

	if(mState1 == SELECT_DONE && mState2 == SELECT_DONE)
	{
		mManager->popScreen();
		mManager->pushScreen(new StageScreen(mPlayer1, mPlayer2, mMusic, mTrainingMode));
	}

	mPlayer1->update();

	if(mPlayer2 != NULL)
		mPlayer2->update();
	else
		waitForInputP2();
}

//--------------------------------------------
void SelectCharScreen::selectChar()
{
	if(mState1 == SELECT_CHAR)
	{
		if(mPlayer1->getInput()->getInputDown(RIGHT))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			if(mCurrentPosition1 > 0)
			{
				mCurrentPosition1--;

				if(mNextPosition1 == 0)
					mNextPosition1 = mFrise1.size() - 1;
				else
					mNextPosition1--;

				if(mPreviousPosition1 == 0)
					mPreviousPosition1 = mFrise1.size() - 1;
				else
					mPreviousPosition1--;
			}
			else
			{
				mCurrentPosition1 = mFrise1.size() - 1;
				mPreviousPosition1--;
				mNextPosition1 = 0;
			}

			mFond1 = mListTextureFond[mCurrentPosition1];
			mName1.SetString(mFrise1Name[mCurrentPosition1]);
			mName1.SetPosition(SMFFEConfig::instance().getViewWidth() * -0.25f - mName1.GetRect().Width *0.5f, 30);
		}

		if(mPlayer1->getInput()->getInputDown(LEFT))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			if(mCurrentPosition1 < mFrise1.size() - 1)
			{
				mCurrentPosition1 ++;

				if(mNextPosition1 == mFrise1.size() - 1)
					mNextPosition1 = 0;
				else
					mNextPosition1++;

				if(mPreviousPosition1 == mFrise1.size() - 1)
					mPreviousPosition1 = 0;
				else
					mPreviousPosition1++;

			}
			else
			{
				mCurrentPosition1 = 0;
				mPreviousPosition1 = mFrise1.size() - 1;
				mNextPosition1++;
			}

			mFond1 = mListTextureFond[mCurrentPosition1];
			mName1.SetString(mFrise1Name[mCurrentPosition1]);
			mName1.SetPosition(SMFFEConfig::instance().getViewWidth() * -0.25f - mName1.GetRect().Width *0.5f, 30);
		}

		if(mPlayer1->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation+1)%3;

			mState1 = SELECT_ULTRA;
			mPlayer1Select = mFrise1Name[mCurrentPosition1];
		}
	}

	if(mState2 == SELECT_CHAR && mPlayer2 != NULL)
	{
		if(mPlayer2->getInput()->getInputDown(RIGHT))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			if(mCurrentPosition2 > 0)
			{
				mCurrentPosition2--;

				if(mNextPosition2 == 0)
					mNextPosition2 = mFrise2.size() - 1;
				else
					mNextPosition2--;

				if(mPreviousPosition2 == 0)
					mPreviousPosition2 = mFrise2.size() - 1;
				else
					mPreviousPosition2--;
			}
			else
			{
				mCurrentPosition2 = mFrise2.size() - 1;
				mPreviousPosition2--;
				mNextPosition2 = 0;
			}

			mFond2 = mListTextureFond[mCurrentPosition2];
			mName2.SetString(mFrise2Name[mCurrentPosition2]);
			mName2.SetPosition(SMFFEConfig::instance().getViewWidth() * 0.25f - mName2.GetRect().Width *0.5f, 30);
		}

		if(mPlayer2->getInput()->getInputDown(LEFT))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			if(mCurrentPosition2 < mFrise2.size() - 1)
			{
				mCurrentPosition2 ++;

				if(mNextPosition2 == mFrise2.size() - 1)
					mNextPosition2 = 0;
				else
					mNextPosition2++;

				if(mPreviousPosition2 == mFrise2.size() - 1)
					mPreviousPosition2 = 0;
				else
					mPreviousPosition2++;
			}
			else
			{
				mCurrentPosition2 = 0;
				mPreviousPosition2 = mFrise2.size() - 1;
				mNextPosition2++;
			}

			mFond2 = mListTextureFond[mCurrentPosition2];
			mName2.SetString(mFrise2Name[mCurrentPosition2]);
			mName2.SetPosition(SMFFEConfig::instance().getViewWidth() * 0.25f - mName2.GetRect().Width *0.5f, 30);
		}

		if(mPlayer2->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation+1)%3;

			mState2 = SELECT_ULTRA;
			mPlayer2Select = mFrise2Name[mCurrentPosition2];
		}
	}
}


//--------------------------------------------
void SelectCharScreen::selectUltra()
{
	if(mState1 == SELECT_ULTRA)
	{
		if(mPlayer1->getInput()->getInputDown(UP))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			mUltraCasu1 = true;
			mUltraSelect1->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), (mUltra1->getSFSprite().GetSize().y * -0.5f)));
		}

		if(mPlayer1->getInput()->getInputDown(DOWN))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			mUltraCasu1 = false;
			mUltraSelect1->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.25f + (mUltra1->getSFSprite().GetSize().x * -0.5f), mUltra1->getSFSprite().GetSize().y * -0.25));
		}

		if(mPlayer1->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation+1)%3;

			mState1 = SELECT_DONE;
			//mPlayer1->setCharacter(mPlayer1Select, !mUltraCasu1);

			SMFFEConfig::instance().sChar1 = mPlayer1Select;
			SMFFEConfig::instance().sCasu1 = !mUltraCasu1;

			if(mTrainingMode)
			{
				mTemp		= mPlayer2;
				mPlayer2	= mPlayer1;
				mPlayer1	= mTemp;
			}
		}
	}

	if(mState2 == SELECT_ULTRA && mPlayer2 != NULL)
	{
		if(mPlayer2->getInput()->getInputDown(UP))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			mUltraCasu2 = true;
			mUltraSelect2->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f + (mUltra2->getSFSprite().GetSize().x * -0.5f), (mUltra2->getSFSprite().GetSize().y * -0.5f)));
		}

		if(mPlayer2->getInput()->getInputDown(DOWN))
		{
			mNavigation[mCurrentNavigation].Play();
			mCurrentNavigation = (mCurrentNavigation+1)%3;

			mUltraCasu2 = false;
			mUltraSelect2->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f + (mUltra2->getSFSprite().GetSize().x * -0.5f), (mUltra2->getSFSprite().GetSize().y * -0.25f)));
		}

		if(mPlayer2->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation+1)%3;

			if(mTrainingMode)
			{
				//mTemp->setCharacter(mPlayer2Select, !mUltraCasu2);

				SMFFEConfig::instance().sChar2 = mPlayer2Select;
				SMFFEConfig::instance().sCasu2 = !mUltraCasu2;

				mPlayer1 = mPlayer2;
				mPlayer2 = mTemp;
			}
			else
			{
				SMFFEConfig::instance().sChar2 = mPlayer2Select;
				SMFFEConfig::instance().sCasu2 = !mUltraCasu2;

				//mPlayer2->setCharacter(mPlayer2Select, !mUltraCasu2);
			}

			mState2 = SELECT_DONE;
		}
	}
}

//--------------------------------------------
void SelectCharScreen::draw(sf::RenderTarget* pTarget)
{
	pTarget->Clear(sf::Color(0, 0, 0, 255));

	mBackground.draw(pTarget);
	mUpperBackground.draw(pTarget);
	mScrollingText.draw(pTarget);

	mFond1->flipX(false);
	mFond1->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.25f - mFond1->getSFSprite().GetSize().x * 0.5f, -250));
	mFond1->draw(pTarget);

	mFond2->flipX(true);
	mFond2->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * 0.25f + mFond2->getSFSprite().GetSize().x * 0.5f, -250));
	mFond2->draw(pTarget);

	if(!mTrainingMode && mPlayer2 == NULL)
	{
		mPressLp.draw(pTarget);
	}

	
	mFrise1[mPreviousPosition1]->getSFSprite().SetScale(0.6f, 0.6f);
	mFrise1[mPreviousPosition1]->setPosition(mPosition1[0]);
	mFrise1[mPreviousPosition1]->getSFSprite().SetColor(sf::Color(255,255,255,50));
	mFrise1[mPreviousPosition1]->draw(pTarget);

	mFrise1[mCurrentPosition1]->getSFSprite().SetScale(1.0f, 1.0f);
	mFrise1[mCurrentPosition1]->setPosition(mPosition1[1]);
	mFrise1[mCurrentPosition1]->getSFSprite().SetColor(sf::Color(255,255,255,255));
	mFrise1[mCurrentPosition1]->draw(pTarget);

	mFrise1[mNextPosition1]->getSFSprite().SetScale(0.6f, 0.6f);
	mFrise1[mNextPosition1]->setPosition(mPosition1[2]);
	mFrise1[mNextPosition1]->getSFSprite().SetColor(sf::Color(255,255,255,50));
	mFrise1[mNextPosition1]->draw(pTarget);

	mFrise2[mPreviousPosition2]->getSFSprite().SetScale(0.6f, 0.6f);
	mFrise2[mPreviousPosition2]->setPosition(mPosition2[0]);
	mFrise2[mPreviousPosition2]->getSFSprite().SetColor(sf::Color(255,255,255,50));
	mFrise2[mPreviousPosition2]->draw(pTarget);

	mFrise2[mCurrentPosition2]->getSFSprite().SetScale(1.0f, 1.0f);
	mFrise2[mCurrentPosition2]->setPosition(mPosition2[1]);
	mFrise2[mCurrentPosition2]->getSFSprite().SetColor(sf::Color(255,255,255,255));
	mFrise2[mCurrentPosition2]->draw(pTarget);

	mFrise2[mNextPosition2]->getSFSprite().SetScale(0.6f, 0.6f);
	mFrise2[mNextPosition2]->setPosition(mPosition2[2]);
	mFrise2[mNextPosition2]->getSFSprite().SetColor(sf::Color(255,255,255,50));
	mFrise2[mNextPosition2]->draw(pTarget);

	pTarget->Draw(mName1);
	pTarget->Draw(mName2);
	

	if(mState1 == SELECT_ULTRA)
	{
		mUltra1->getSFSprite().SetColor(sf::Color(255,255,255,255));
		mUltra1->draw(pTarget);
		mUltraSelect1->getSFSprite().SetColor(sf::Color(255,255,255,128));
		mUltraSelect1->draw(pTarget);
	}

	if(mState2 == SELECT_ULTRA)
	{
		mUltra2->getSFSprite().SetColor(sf::Color(255,255,255,255));
		mUltra2->draw(pTarget);
		mUltraSelect2->getSFSprite().SetColor(sf::Color(255,255,255,128));
		mUltraSelect2->draw(pTarget);
	}

	if(mState1 == SELECT_DONE)
	{
		mUltra1->getSFSprite().SetColor(sf::Color(255,255,255,0));
		mUltra1->draw(pTarget);
		mUltraSelect1->getSFSprite().SetColor(sf::Color(255,255,255,0));
		mUltraSelect1->draw(pTarget);
	}

	if(mState2 == SELECT_DONE)
	{
		mUltra2->getSFSprite().SetColor(sf::Color(255,255,255,0));
		mUltra2->draw(pTarget);
		mUltraSelect2->getSFSprite().SetColor(sf::Color(255,255,255,0));
		mUltraSelect2->draw(pTarget);
	}
}

void SelectCharScreen::waitForInputP2()
{
	for(int i = 0; i < 8; i++)
	{
		if(sf::Joystick::IsConnected(i))
		{
			if(mPlayer1->getInput()->getType() == "joystick" && mPlayer1->getInput()->getNumber() == i)
			{
				continue;
			}


			if(sf::Joystick::IsButtonPressed(i, SMFFEConfig::instance().getJoypadMapping(i)[C_LP]))
			{
				mPlayer2 = new Player(new JoypadInput(i), false);
			}
		}
	}

	if(mPlayer1->getInput()->getType() != "keyboard")
	{
		if(sf::Keyboard::IsKeyPressed((sf::Keyboard::Key)SMFFEConfig::instance().getKeyboardMapping()[C_LP]))
		{
			mPlayer2 = new Player(new KeyboardInput(), false);
		}
	}
}