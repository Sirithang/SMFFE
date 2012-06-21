#include "..\include\JoypadInput.h"

#include "SMFFEConfig.h"

#include <SFML/Window.hpp>

#include <iostream>

using namespace sf;

JoypadInput::JoypadInput(int pNumber)
	:CommandInput()
{
	mNumber = pNumber;
	mType = "joystick";
}


JoypadInput::~JoypadInput(void)
{
}

//-----------------------------------------

void JoypadInput::pollImplementation()
{
	int lPushedKey = 0;

	Joystick::Update();

	system("cls");

	if(Joystick::GetAxisPosition(mNumber, Joystick::Y) < -40.0f )
	{
		lPushedKey |= UP;
	}

	if(Joystick::GetAxisPosition(mNumber, Joystick::Y) > 40.0f)
	{
		lPushedKey |= DOWN;
	}

	if(Joystick::GetAxisPosition(mNumber, Joystick::X) < -40.0f)
	{
		lPushedKey |= LEFT;
	}
	
	if(Joystick::GetAxisPosition(mNumber, Joystick::X) > 40.0f)
	{
		lPushedKey |= RIGHT;
	}

	if(Joystick::GetAxisPosition(mNumber, Joystick::PovY) != 0 || Joystick::GetAxisPosition(mNumber, Joystick::PovX) != 0)
	{
		float lAngle = std::atan2f(Joystick::GetAxisPosition(mNumber, Joystick::PovY) , Joystick::GetAxisPosition(mNumber, Joystick::PovX));

		//std::cout<<lAngle<<std::endl;

		if((lAngle < 0.5f || lAngle > 2.6f))
		{
			lPushedKey |= UP;
		}

		if((lAngle > 0.8f && lAngle < 2.0f))
		{
			lPushedKey |= DOWN;
		}

		if((lAngle > 1.6f && lAngle < 2.8f))
		{
			lPushedKey |= LEFT;
		}
	
		if((lAngle > 0.2f && lAngle < 1.2f))
		{
			lPushedKey |= RIGHT;
		}
	}

	//---------------punch/kick

	std::map<InputType, int> lMapping = SMFFEConfig::instance().getJoypadMapping(mNumber);

	if(Joystick::IsButtonPressed(mNumber, lMapping[C_LK]))
	{
		lPushedKey |= C_LK;
	}

	if(Joystick::IsButtonPressed(mNumber, lMapping[C_HK]))
	{
		lPushedKey |= C_HK;
	}

	if(Joystick::IsButtonPressed(mNumber, lMapping[C_LP]))
	{
		lPushedKey |= C_LP;
	}

	if(Joystick::IsButtonPressed(mNumber, lMapping[C_HP]))
	{
		lPushedKey |= C_HP;
	}

	if(Joystick::IsButtonPressed(mNumber, lMapping[START]))
	{
		lPushedKey |= START;
	}

	if(Joystick::IsButtonPressed(mNumber, lMapping[BACK]))
	{
		lPushedKey |= BACK;
	}

	pushed((InputType)lPushedKey);
}