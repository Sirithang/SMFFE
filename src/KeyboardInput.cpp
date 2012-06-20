#include "..\include\KeyboardInput.h"
#include <SFML/Window/Keyboard.hpp>

#include "SMFFEConfig.h"

using namespace sf;

KeyboardInput::KeyboardInput(void)
	:CommandInput()
{
	mNumber = 0;
	mType = "keyboard";
}


KeyboardInput::~KeyboardInput(void)
{
}

void KeyboardInput::pollImplementation()
{
	// Si le buffer depasse 20 input, on pop la derniere
	/*if(mCommandBuffer.size() > 20)
	{
		mCommandBuffer.pop_back();
	}
	
	int lPushedKey = 0;
	const int lNumKey = 4;

	if(Keyboard::IsKeyPressed(Keyboard::Up))
	{
		if(Keyboard::IsKeyPressed(Keyboard::Left))
		{
			if(mLastDirection != UPLEFT)
				lPushedKey |= UPLEFT;

			mLastDirection = UPLEFT;
		}
		else if(Keyboard::IsKeyPressed(Keyboard::Right))
		{
			if(mLastDirection != UPRIGHT)
				lPushedKey |= UPRIGHT;

			mLastDirection = UPRIGHT;
		}
		else
		{
			if(mLastDirection != UP)
				lPushedKey |= UP;

			mLastDirection = UP;
		}
	}
	else if(Keyboard::IsKeyPressed(Keyboard::Down))
	{
		if(Keyboard::IsKeyPressed(Keyboard::Left))
		{
			if(mLastDirection != DOWNLEFT)
				lPushedKey |= DOWNLEFT;
			
			mLastDirection = DOWNLEFT;
		}
		else if(Keyboard::IsKeyPressed(Keyboard::Right))
		{
			if(mLastDirection != DOWNRIGHT)
				lPushedKey |= DOWNRIGHT;

			mLastDirection = DOWNRIGHT;
		}
		else
		{
			if(mLastDirection != DOWN)
				lPushedKey |= DOWN;

			mLastDirection = DOWN;
		}
	}
	else
	{
		if(Keyboard::IsKeyPressed(Keyboard::Left))
		{
			if(mLastDirection != LEFT)
				lPushedKey |= LEFT;

			mLastDirection = LEFT;
		}
		else if(Keyboard::IsKeyPressed(Keyboard::Right))
		{
			if(mLastDirection != RIGHT)
				lPushedKey |= RIGHT;

			mLastDirection = RIGHT;
		}
		else
		{
			mLastDirection = NONE;
		}
	}


	//---------------punch/kick

	if(Keyboard::IsKeyPressed(Keyboard::A))
	{
		if(!mKeyStats[LP])
		{
			lPushedKey |= LP;
		}
		mKeyStats[LP] = true;
	}
	else
	{
		mKeyStats[LP] = false;
	}


	if(lPushedKey != 0x0)
	{
		mCommandBuffer.push_back((InputType)lPushedKey);
		mTTLCommand.push_back(0.5f);
	}*/

	int lPushedKey = 0;

	if(Keyboard::IsKeyPressed(Keyboard::Up))
	{
		lPushedKey |= UP;
	}
	
	if(Keyboard::IsKeyPressed(Keyboard::Down))
	{
		lPushedKey |= DOWN;
	}

	if(Keyboard::IsKeyPressed(Keyboard::Left))
	{
		lPushedKey |= LEFT;
	}
	
	if(Keyboard::IsKeyPressed(Keyboard::Right))
	{
		lPushedKey |= RIGHT;
	}

	//---------------punch/kick

	std::map<InputType, int> lKeys = SMFFEConfig::instance().getKeyboardMapping();

	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[C_LP]))
	{
		lPushedKey |= C_LP;
	}
	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[C_HP]))
	{
		lPushedKey |= C_HP;
	}
	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[C_LK]))
	{
		lPushedKey |= C_LK;
	}
	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[C_HK]))
	{
		lPushedKey |= C_HK;
	}

	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[BACK]))
	{
		lPushedKey |= BACK;
	}

	if(Keyboard::IsKeyPressed((sf::Keyboard::Key)lKeys[START]))
	{
		lPushedKey |= START;
	}

	pushed((InputType)lPushedKey);
}