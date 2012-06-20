#ifndef SMFFE_PLAYER_H
#define SMFFE_PLAYER_H

#include "CommandInput.h"

#include "Character.h"

class Player
{
protected:
	CommandInput* mInput;
	Character* mChar;

	bool mLeftSide;
	bool mUpdatePlayer;

	sf::String mCharpath;

	sf::Thread* lThread;

public:
	Player(CommandInput* pInput, bool pLeftSide);
	~Player(void);

	CommandInput* getInput() { return mInput; }

	void setCharacter(sf::String pChar, bool pCasu);

	void update();
	void draw(sf::RenderTarget* pTarget);

	bool isLeftSide() { return mLeftSide; }

	void setUpdatePlayer(bool pUpdate) { mUpdatePlayer = pUpdate; }

	Character* getCharacter() { return mChar; }

	void loadChar() { mChar->loadFromFolder(mCharpath); }

	void deleteChar();
};

#endif