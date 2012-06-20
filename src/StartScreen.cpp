#include "..\include\StartScreen.h"
#include "SMFFETime.h"

#include "constants.h"

#include "MenuScreen.h"

#include "SMFFEConfig.h"

#include "KeyboardInput.h"
#include "JoypadInput.h"

StartScreen::StartScreen(void)
{
	mMusique = new sf::Music();
	mMusique->OpenFromFile("data/screens/startScreen/music.ogg");

	if(SMFFEConfig::instance().mMusicOn)
	{
		mMusique->SetVolume(50);
	}
	else
	{
		mMusique->SetVolume(0);
	}

	mMusique->Play();

	mBuffer.LoadFromFile("data/screens/startScreen/start.ogg");
	mSound.SetBuffer(mBuffer);

	mSound.SetVolume(50);

	mBackground.LoadFromFile("data/screens/startScreen/titre_mc.png");
	mBackgroundSpr.SetTexture(mBackground);
	mBackgroundSpr.SetPosition(- sf::Vector2f(mBackgroundSpr.GetSize().x * 0.5f, mBackgroundSpr.GetSize().y  * 0.5f));

	mStartTexture.LoadFromFile("data/screens/startScreen/PressStart.png");
	mStartSprite.SetTexture(mStartTexture);

	mStartSprite.SetPosition(mStartTexture.GetWidth() * -0.5f, SMFFEConfig::instance().getViewHeight()*0.5f - mStartTexture.GetHeight());

	mMulti = -1;

	mView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(SMFFEConfig::instance().getViewWidth(), SMFFEConfig::instance().getViewHeight()));

	for(int i = 0; i < 8; i++)
	{
		if(sf::Joystick::IsConnected(i))
		{
			mJoypads.push_back(i);
		}
	}
}


StartScreen::~StartScreen(void)
{
	//delete mMusique;
}


//--------------------------------------------


void StartScreen::update()
{
	int alpha = mStartSprite.GetColor().a + (255 * mMulti * Time::deltaTime());

	if(alpha <= 0)
	{
		mMulti = 1;
		alpha = 0;
	}
	else if (alpha >= 255)
	{
		mMulti = -1;
		alpha = 255;
	}

	mStartSprite.SetColor(sf::Color(255, 255, 255, alpha));

	if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Return))
	{
		mSound.Play();
		mManager->popScreen();
		mManager->pushScreen(new MenuScreen(new KeyboardInput(), mMusique));
		return;
	}

	for(int i  = 0; i < mJoypads.size(); i++)
	{
		if(sf::Joystick::IsButtonPressed(mJoypads.at(i), SMFFEConfig::instance().getJoypadMapping(i)[START]))
		{
			mSound.Play();
			mManager->popScreen();
			mManager->pushScreen(new MenuScreen(new JoypadInput(mJoypads.at(i)), mMusique));
			return;
		}
	}
}

//--------------------------------------------

void StartScreen::draw(sf::RenderTarget* pTarget)
{
	pTarget->Clear(sf::Color(0, 0, 0, 255));
	pTarget->Draw(mBackgroundSpr);
	pTarget->Draw(mStartSprite);
}