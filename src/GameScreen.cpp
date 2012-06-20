#include "GameScreen.h"
#include "KeyboardInput.h"
#include "constants.h"


GameScreen::GameScreen(const std::string& pStage, const std::string& pP1, const std::string& pP2)
{
	/*mStageTexture.LoadFromFile(gStagePath+pStage);
	mStageSprite.SetTexture(mStageTexture);
	mStageSprite.SetPosition(-100.0f,-100.0f);*/

	mPlayer1 = new Player(new KeyboardInput(), new Character(pP1,sf::Vector2f(0.0f,0.0f)));
	mPlayer2 = new Player(new KeyboardInput(), new Character(pP2,sf::Vector2f(700.0f,0.0f)));
}


GameScreen::~GameScreen()
{
}

//--------------------------------------------

void GameScreen::update()
{
	mStageSprite.SetColor(sf::Color(255, 255, 255, 255));
	mPlayer1->update();
	mPlayer2->update();
}

//--------------------------------------------

void GameScreen::draw(sf::RenderTarget* pTarget)
{
	pTarget->Clear(sf::Color(255, 255, 255, 255));/*
	pTarget->Draw(mStageSprite);*/
	mPlayer1->draw(pTarget);
	mPlayer2->draw(pTarget);
}