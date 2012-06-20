#include "..\include\CreditScreen.h"
#include "SMFFEConfig.h"
#include "SMFFETime.h"

CreditScreen::CreditScreen(Player* pPlayer)
	:mPlayer(pPlayer),
	mMorgane("data/screens/creditScreen/morgane.png", sf::Vector2f(0,0)),
	mDavid("data/screens/creditScreen/david.png", sf::Vector2f(0,0)),
	mMathieu("data/screens/creditScreen/mathieu.png", sf::Vector2f(0,0)),
	mAlexandra("data/screens/creditScreen/alex.png", sf::Vector2f(0,0)),
	mMichel("data/screens/creditScreen/mica.png", sf::Vector2f(0,0)),
	mSimon("data/screens/creditScreen/simon.png", sf::Vector2f(0,0)),
	mGuillaume("data/screens/creditScreen/guillaume.png", sf::Vector2f(0,0)),
	mBackground("data/screens/creditScreen/background.png", sf::Vector2f(0,0))
{
	setDefaultPos();

	mBackground.setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() * -0.5f,
		SMFFEConfig::instance().getViewHeight() * -0.5f));

	mFont.LoadFromFile("data/font/graph_35_pix.ttf");
	mText.SetFont(mFont);
	mText.SetString("Escap to exit");
	mText.SetPosition(	- SMFFEConfig::instance().getViewWidth() * 0.5f,
						- SMFFEConfig::instance().getViewHeight() * 0.5f);
}


CreditScreen::~CreditScreen(void)
{
	
}

//------------------------------------------------------

void CreditScreen::update()
{
	const float lSpeed = 60;

	mMorgane.setPosition(mMorgane.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mDavid.setPosition(mDavid.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mMathieu.setPosition(mMathieu.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mAlexandra.setPosition(mAlexandra.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mMichel.setPosition(mMichel.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mSimon.setPosition(mSimon.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed)); 
	mGuillaume.setPosition(mGuillaume.getPosition() - sf::Vector2f(0, Time::deltaTime() * lSpeed));

	if(mGuillaume.getPosition().y < SMFFEConfig::instance().getWinHeight() * -0.5f)
		setDefaultPos();

	mPlayer->getInput()->pollData();

	if(mPlayer->getInput()->getInputDown(C_LP) || mPlayer->getInput()->getInputDown(BACK))
	{
		mManager->popScreen();
		mManager->pushScreen(new MenuScreen(mPlayer->getInput()));
	}
}

//------------------------------------------------------

void CreditScreen::draw(sf::RenderTarget* pTarget)
{
	mBackground.draw(pTarget);

	mMorgane.draw(pTarget);
	mDavid.draw(pTarget);
	mMathieu.draw(pTarget);
	mAlexandra.draw(pTarget);
	mMichel.draw(pTarget);
	mSimon.draw(pTarget);
	mGuillaume.draw(pTarget);

	pTarget->Draw(mText);
}

//------------------------------------------------------

void CreditScreen::setDefaultPos()
{
	mMorgane.setPosition(sf::Vector2f(	mMorgane.getSFSprite().GetSize().x * -0.5f,
										SMFFEConfig::instance().getViewHeight()	* 0.5f));
	mDavid.setPosition(sf::Vector2f(	mDavid.getSFSprite().GetSize().x * -0.5f,
										mMorgane.getPosition().y + 100));
	mMathieu.setPosition(sf::Vector2f(	mMathieu.getSFSprite().GetSize().x * -0.5f,
										mDavid.getPosition().y + 100));
	mAlexandra.setPosition(sf::Vector2f(mAlexandra.getSFSprite().GetSize().x * -0.5f,
										mMathieu.getPosition().y + 100));
	mMichel.setPosition(sf::Vector2f(	mMichel.getSFSprite().GetSize().x * -0.5f,
										mAlexandra.getPosition().y + 100));
	mSimon.setPosition(sf::Vector2f(	mSimon.getSFSprite().GetSize().x * -0.5f,
										mMichel.getPosition().y + 100));
	mGuillaume.setPosition(sf::Vector2f(mGuillaume.getSFSprite().GetSize().x * -0.5f,
										mSimon.getPosition().y + 100));
}