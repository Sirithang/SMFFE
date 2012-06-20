#ifndef SMFFE_GAMESCREEN_H
#define SMFFE_GAMESCREEN_H

#include <SFML/Graphics.hpp>

#include "AScreen.h"
#include "Player.h"

class GameScreen: public AScreen
{
private:
	sf::Texture mStageTexture;
	sf::Sprite mStageSprite;

	Player* mPlayer1;
	Player* mPlayer2;

public:
	GameScreen(const std::string& pStage, const std::string& pP1, const std::string& pP2);
	~GameScreen();
	
	void update();
	void draw(sf::RenderTarget* pTarget);

};

#endif