#include "..\include\OptionScreen.h"

#include "SMFFEConfig.h"
#include "SMFFETime.h"
#include "helpers.h"

OptionScreen::OptionScreen(Player* pPlayer, sf::Music* pMusic)
	:	mPlayer(pPlayer),
	mBackground("data/screens/optionScreen/options.png", sf::Vector2f(0,0)),
	mFlecheSelection("data/screens/optionScreen/fleche.png", sf::Vector2f(0,0)),
	mMusic(pMusic)
{
	mBackground.setPosition(mBackground.getSFSprite().GetSize() * -0.5f);

	mNavigationBuffer.LoadFromFile("data/screens/SelectCharScreen/navigation.ogg");
	mValidationBuffer.LoadFromFile("data/screens/SelectCharScreen/validation.ogg");

	for(int i = 0; i < 3; i++)
	{
		mNavigation[i].SetBuffer(mNavigationBuffer);
		mValidation[i].SetBuffer(mValidationBuffer);
	}

	mCurrentNavigation = 0;
	mCurrentValidation = 0;

	//**
	mNbSelection  = 7;
	mCurrentSelection = 0;

	mFont.LoadFromFile("data/font/graph_35_pix.ttf");

	mTitle.SetFont(mFont);
	mTitle.SetColor(sf::Color::White);
	mTitle.SetScale(2, 2);
	mTitle.SetString("-- Options --");
	mTitle.SetPosition( mTitle.GetRect().Width * -0.5f,
		SMFFEConfig::instance().getViewHeight() * -0.5f + mTitle.GetRect().Height * 0.5f);

	mFullscreen.SetFont(mFont);
	mFullscreen.SetScale(0.7f, 0.7f);
	mFullscreen.SetColor(sf::Color::White);
	mFullscreen.SetPosition(SMFFEConfig::instance().getViewWidth() * -0.5f + 100,
							SMFFEConfig::instance().getViewHeight() * -0.5f + 200);

	mResolution.SetFont(mFont);
	mResolution.SetScale(0.7f, 0.7f);
	if(!SMFFEConfig::instance().isFullscreen())
		mResolution.SetColor(sf::Color::White);
	else
		mResolution.SetColor(sf::Color(100, 100, 100));

	mResolution.SetPosition(mFullscreen.GetPosition() + sf::Vector2f(0, 50));


	mMusicTxt.SetFont(mFont);
	mMusicTxt.SetScale(0.7f, 0.7f);
	mMusicTxt.SetColor(sf::Color::White);
	mMusicTxt.SetPosition(mResolution.GetPosition() + sf::Vector2f(0, 50));

	if(SMFFEConfig::instance().mMusicOn)
	{
		mMusicTxt.SetString("Music : Yes");
	}
	else
	{
		mMusicTxt.SetString("Music : No");
	}


	mMapping.SetFont(mFont);
	mMapping.SetScale(1.2f, 1.2f);
	mMapping.SetColor(sf::Color::White);
	mMapping.SetString("KEY MAPPING : ");
	mMapping.SetPosition(mResolution.GetPosition() + sf::Vector2f(0, 100));

	mLP.SetFont(mFont);
	mLP.SetScale(0.7f, 0.7f);
	mLP.SetColor(sf::Color::White);
	mLP.SetPosition(mMapping.GetPosition() + sf::Vector2f(0, 60));

	mHP.SetFont(mFont);
	mHP.SetScale(0.7f, 0.7f);
	mHP.SetColor(sf::Color::White);
	mHP.SetPosition(mLP.GetPosition() + sf::Vector2f(0, 50));

	mLK.SetFont(mFont);
	mLK.SetScale(0.7f, 0.7f);
	mLK.SetColor(sf::Color::White);
	mLK.SetPosition(mHP.GetPosition() + sf::Vector2f(0, 50));

	mHK.SetFont(mFont);
	mHK.SetScale(0.7f, 0.7f);
	mHK.SetColor(sf::Color::White);
	mHK.SetPosition(mLK.GetPosition() + sf::Vector2f(0, 50));

	mMappingActual = SMFFEConfig::instance().getMappingForInput(mPlayer->getInput());
	mWaitingForKey = false;
	moveSelector();
}


OptionScreen::~OptionScreen(void)
{
	SMFFEConfig::instance().save();
}

//--------------------------------------------------------------'

void OptionScreen::update()
{
	mPlayer->getInput()->pollData();

	if(mWaitingForKey)
	{
		WaitForKey();
		return;
	}

	if(mPlayer->getInput()->getInputDown(C_HK) || mPlayer->getInput()->getInputDown(BACK) || (mPlayer->getInput()->getInputDown(START) && mCurrentSelection < 3))
	{
		mManager->popScreen();
		return;
	}

	if(mPlayer->getInput()->getInputDown(UP))
	{
		mCurrentSelection--;
		if(mCurrentSelection < 0) mCurrentSelection += mNbSelection;
		moveSelector();
	}
	else if(mPlayer->getInput()->getInputDown(DOWN))
	{
		mCurrentSelection++;
		if(mCurrentSelection >= mNbSelection) mCurrentSelection -= mNbSelection;
		moveSelector();
	}

	handleInput();

	
	
	mFullscreen.SetString(sf::String("Fullscreen - ") + (SMFFEConfig::instance().isFullscreen()?sf::String("Yes"):sf::String("No")));

	mResolution.SetString(sf::String("Resolution (windowed only) - ") + intToString(SMFFEConfig::instance().getWinWidth())
							+sf::String("x")+intToString(SMFFEConfig::instance().getWinHeight()));


	if(mPlayer->getInput()->getType() == "keyboard")
	{
		mLP.SetString(sf::String("LP - ") + getNameOfKey((sf::Keyboard::Key)mMappingActual[C_LP]));
		mHP.SetString(sf::String("HP - ") + getNameOfKey((sf::Keyboard::Key)mMappingActual[C_HP]));
		mLK.SetString(sf::String("LK - ") + getNameOfKey((sf::Keyboard::Key)mMappingActual[C_LK]));
		mHK.SetString(sf::String("HK - ") + getNameOfKey((sf::Keyboard::Key)mMappingActual[C_HK]));
	}
	else
	{
		mLP.SetString(sf::String("LP - ") + intToString(mMappingActual[C_LP]));
		mHP.SetString(sf::String("HP - ") + intToString(mMappingActual[C_HP]));
		mLK.SetString(sf::String("LK - ") + intToString(mMappingActual[C_LK]));
		mHK.SetString(sf::String("HK - ") + intToString(mMappingActual[C_HK]));
	}

}

//--------------------------------------------------------------

void OptionScreen::draw(sf::RenderTarget* pTarget)
{
	mBackground.draw(pTarget);

	pTarget->Draw(mTitle);
	pTarget->Draw(mFullscreen);
	pTarget->Draw(mResolution);
	pTarget->Draw(mMusicTxt);

	pTarget->Draw(mMapping);
	pTarget->Draw(mLP);
	pTarget->Draw(mHP);
	pTarget->Draw(mLK);
	pTarget->Draw(mHK);

	mFlecheSelection.draw(pTarget);
}

//--------------------------------------------------------

void OptionScreen::moveSelector()
{
	float lOffset = 45;
	float lOffsetY = 14;

	mNavigation[mCurrentNavigation].Play();
	mCurrentNavigation = (mCurrentNavigation + 1)%3;

	switch(mCurrentSelection)
	{
	case 0:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mFullscreen.GetPosition().y - lOffsetY));
		break;
	case 1:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mResolution.GetPosition().y - lOffsetY));
		break;
	case 2:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mMusicTxt.GetPosition().y - lOffsetY));
		break;
	case 3:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mLP.GetPosition().y - lOffsetY));
		break;
	case 4:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mHP.GetPosition().y - lOffsetY));
		break;
	case 5:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mLK.GetPosition().y - lOffsetY));
		break;
	case 6:
		mFlecheSelection.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f + lOffset, mHK.GetPosition().y - lOffsetY));
		break;
	default:
		break;
	}
}

//--------------------------------------------------------

void OptionScreen::handleInput()
{
	if(mCurrentSelection == 0)
	{
		if(mPlayer->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation + 1)%3;

			SMFFEConfig::instance().setFullscreen(!SMFFEConfig::instance().isFullscreen());

			if(SMFFEConfig::instance().isFullscreen())
			{
				mManager->getWin()->Create(sf::VideoMode(	sf::VideoMode::GetDesktopMode().Width,
															sf::VideoMode::GetDesktopMode().Height, 32),
															"SMFFE",
															sf::Style::Fullscreen);
			
				mResolution.SetColor(sf::Color(100, 100, 100));
			}
			else
			{
				mManager->getWin()->Create(sf::VideoMode(	SMFFEConfig::instance().getWinWidth(),
															SMFFEConfig::instance().getWinHeight(), 32),
															"SMFFE",
															sf::Style::Default);
			
				mResolution.SetColor(sf::Color::Black);
			}

			mManager->getWin()->SetView(mView);
		}
	}
	else if(mCurrentSelection == 1)
	{
	}
	else if(mCurrentSelection == 2)
	{
		if(mPlayer->getInput()->getInputDown(C_LP))
		{
			mValidation[mCurrentValidation].Play();
			mCurrentValidation = (mCurrentValidation + 1)%3;

			SMFFEConfig::instance().mMusicOn = !SMFFEConfig::instance().mMusicOn;

			if(SMFFEConfig::instance().mMusicOn)
			{
				mMusicTxt.SetString("Music : Yes");
				mMusic->SetVolume(100);
			}
			else
			{
				mMusicTxt.SetString("Music : No");
				mMusic->SetVolume(0);
			}
		}
	}
	else if(mCurrentSelection >= 3 && mCurrentSelection < mNbSelection)
	{
		if(mPlayer->getInput()->getInputDown(START))
		{
			mWaitingForKey = true;

			switch(mCurrentSelection)
			{
			case 3:
				mKeyWaited = C_LP;
				mLP.SetString("LP -");
				break;
			case 4:
				mKeyWaited = C_HP;
				mLP.SetString("HP -");
				break;
			case 5:
				mKeyWaited = C_LK;
				mLP.SetString("LK -");
				break;
			case 6:
				mKeyWaited = C_HK;
				mLP.SetString("HK -");
				break;
			default:
				break;
			}

			mTimeSinceLastPushed = 10;
		}
	}

}

//------------------------------------------

void OptionScreen::WaitForKey()
{

	if(!sf::Keyboard::IsKeyPressed((sf::Keyboard::Key)mMappingActual[START]))
		mTimeSinceLastPushed = -10;

	if(mPlayer->getInput()->getType() == "keyboard")
	{
		for(int i = 0; i < sf::Keyboard::KeyCount; i++)
		{
			if(sf::Keyboard::IsKeyPressed((sf::Keyboard::Key)i))
			{
				mMappingActual[mKeyWaited] = i;
				mWaitingForKey = false;
				return;
			}
		}
	}
	else if(mPlayer->getInput()->getType() == "joystick")
	{
		int num = mPlayer->getInput()->getNumber();

		for(int i = 0; i < sf::Joystick::GetButtonCount(num); i++)
		{
			if(sf::Joystick::IsButtonPressed(num, i))
			{
				mMappingActual[mKeyWaited] = i;
				mWaitingForKey = false;
				return;
			}
		}
	}
}