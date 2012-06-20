#include "..\include\CommandInput.h"
#include "SMFFETime.h"



CommandInput::CommandInput()
{
	InputType lInput[14]= {	UP,
							DOWN,
							LEFT,
							RIGHT,
							UPRIGHT,
							UPLEFT,
							DOWNLEFT,
							DOWNRIGHT,
							C_LP,
							C_LK,
							C_HP,
							C_HK,
							START,
							BACK
						};

	for(int i = 0; i < 14; i++)
	{
		mKeyStats[lInput[i]] = false;
	}

	mLastDirection = NONE;
}

//-------------------------------------------

void CommandInput::pollData()
{
	decrementeTTL();
	pollImplementation();
}

void CommandInput::decrementeTTL()
{
	if(mTTLCommand.empty())
		return;

	for(std::list<float>::iterator lIt = mTTLCommand.begin(); lIt != mTTLCommand.end(); lIt++)
	{
		*lIt -= Time::deltaTime();
	}

	while(!mTTLCommand.empty() && (*mTTLCommand.begin()) <= 0)
	{
		mTTLCommand.pop_front();
		mCommandBuffer.pop_front();
	}
}
//-------------------------------------------------

bool CommandInput::getInputDown(InputType pType)
{
	bool lDown = mKeyStats[pType];

	if(lDown)
		mKeyStats[pType] = false;

	return lDown;
}


//-------------------------------------------------

void CommandInput::pushed(InputType pType)
{
	// Si le buffer depasse 20 input, on pop la derniere
	if(mCommandBuffer.size() > 20)
	{
		mCommandBuffer.pop_front();
		mTTLCommand.pop_front();
	}

	int lPushedKey = 0;
	const int lNumKey = 4;

	if(pType&UP)
	{
		mWasReleased[DOWN] = true;
		mKeyStats[DOWN] = false;
		mWasReleased[DOWNLEFT] = true;
		mKeyStats[DOWNLEFT]= false;
		mWasReleased[DOWNRIGHT] = true;
		mKeyStats[DOWNRIGHT] = false;
		mWasReleased[LEFT] = true;
		mKeyStats[LEFT]= false;
		mWasReleased[RIGHT] = true;
		mKeyStats[RIGHT] = false;

		if(pType&LEFT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != UPLEFT)
				lPushedKey |= UPLEFT;

			mLastDirection = UPLEFT;

			//******

			if(mWasReleased[UPLEFT])
			{
				mKeyStats[UPLEFT] = true;
				mWasReleased[UPLEFT] = false;
			}

			mWasReleased[UP] = true;
			mKeyStats[UP] = false;
			mWasReleased[UPRIGHT] = true;
			mKeyStats[UPRIGHT]= false;
		}
		else if(pType&RIGHT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != UPRIGHT)
				lPushedKey |= UPRIGHT;

			mLastDirection = UPRIGHT;

			//******

			if(mWasReleased[UPRIGHT])
			{
				mKeyStats[UPRIGHT] = true;
				mWasReleased[UPRIGHT] = false;
			}

			mWasReleased[UP] = true;
			mKeyStats[UP] = false;
			mWasReleased[UPLEFT] = true;
			mKeyStats[UPLEFT]= false;
		}
		else
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != UP)
				lPushedKey |= UP;

			mLastDirection = UP;

			//******

			if(mWasReleased[UP])
			{
				mKeyStats[UP] = true;
				mWasReleased[UP] = false;
			}

			mWasReleased[UPRIGHT] = true;
			mKeyStats[UPRIGHT] = false;
			mWasReleased[UPLEFT] = true;
			mKeyStats[UPLEFT]= false;
		}
	}
	else if(pType&DOWN)
	{
		mWasReleased[UPRIGHT] = true;
		mKeyStats[UPRIGHT] = false;
		mWasReleased[UPLEFT] = true;
		mKeyStats[UPLEFT]= false;
		mWasReleased[UP] = true;
		mKeyStats[UP] = false;
		mWasReleased[LEFT] = true;
		mKeyStats[LEFT]= false;
		mWasReleased[RIGHT] = true;
		mKeyStats[RIGHT] = false;


		if(pType&LEFT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != DOWNLEFT)
				lPushedKey |= DOWNLEFT;
			
			mLastDirection = DOWNLEFT;

			//******

			if(mWasReleased[DOWNLEFT])
			{
				mKeyStats[DOWNLEFT] = true;
				mWasReleased[DOWNLEFT] = false;
			}

			mWasReleased[DOWN] = true;
			mKeyStats[DOWN] = false;
			mWasReleased[DOWNRIGHT] = true;
			mKeyStats[DOWNRIGHT]= false;
		}
		else if(pType&RIGHT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != DOWNRIGHT)
				lPushedKey |= DOWNRIGHT;

			mLastDirection = DOWNRIGHT;

			//******

			if(mWasReleased[DOWNRIGHT])
			{
				mKeyStats[DOWNRIGHT] = true;
				mWasReleased[DOWNRIGHT] = false;
			}

			mWasReleased[DOWN] = true;
			mKeyStats[DOWN] = false;
			mWasReleased[DOWNLEFT] = true;
			mKeyStats[DOWNLEFT]= false;
		}
		else
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != DOWN)
				lPushedKey |= DOWN;

			mLastDirection = DOWN;

			//******

			if(mWasReleased[DOWN])
			{
				mKeyStats[DOWN] = true;
				mWasReleased[DOWN] = false;
			}

			mWasReleased[DOWNRIGHT] = true;
			mKeyStats[DOWNRIGHT] = false;
			mWasReleased[DOWNLEFT] = true;
			mKeyStats[DOWNLEFT]= false;
		}
	}
	else
	{
		mWasReleased[UPRIGHT] = true;
		mKeyStats[UPRIGHT] = false;
		mWasReleased[UPLEFT] = true;
		mKeyStats[UPLEFT]= false;
		mWasReleased[UP] = true;
		mKeyStats[UP] = false;
		mWasReleased[DOWNLEFT] = true;
		mKeyStats[DOWNLEFT]= false;
		mWasReleased[DOWNRIGHT] = true;
		mKeyStats[DOWNRIGHT] = false;
		mWasReleased[DOWN] = true;
		mKeyStats[DOWN] = false;

		if(pType&LEFT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != UPLEFT)
				lPushedKey |= LEFT;

			mLastDirection = LEFT;

			//**********

			if(mWasReleased[LEFT])
			{
				mKeyStats[LEFT] = true;
				mWasReleased[LEFT] = false;
			}

			mWasReleased[RIGHT] = true;
			mKeyStats[RIGHT] = false;
		}
		else if(pType&RIGHT)
		{
			if(mCommandBuffer.size() == 0 || mCommandBuffer.back() != RIGHT)
				lPushedKey |= RIGHT;

			mLastDirection = RIGHT;

			//**********

			if(mWasReleased[RIGHT])
			{
				mKeyStats[RIGHT] = true;
				mWasReleased[RIGHT] = false;
			}

			mWasReleased[LEFT] = true;
			mKeyStats[LEFT] = false;
		}
		else
		{
			mWasReleased[LEFT] = true;
			mKeyStats[LEFT] = false;
			mWasReleased[RIGHT] = true;
			mKeyStats[RIGHT] = false;

			mLastDirection = NONE;
		}
	}


	//---------------punch/kick

	if(pType&C_LP)
	{
		if(!mKeyStats[C_LP])
		{
			lPushedKey |= C_LP;
		}

		if(mWasReleased[C_LP])
		{
			mKeyStats[C_LP] = true;
			mWasReleased[C_LP] = false;
		}
	}
	else
	{
		mKeyStats[C_LP] = false;
		mWasReleased[C_LP] = true;
	}

	if(pType&C_HP)
	{
		if(!mKeyStats[C_HP])
		{
			lPushedKey |= C_HP;
		}
		
		if(mWasReleased[C_HP])
		{
			mKeyStats[C_HP] = true;
			mWasReleased[C_HP] = false;
		}
	}
	else
	{
		mKeyStats[C_HP] = false;
		mWasReleased[C_HP] = true;
	}

	if(pType&C_LK)
	{
		if(!mKeyStats[C_LK])
		{
			lPushedKey |= C_LK;
		}
		
		if(mWasReleased[C_LK])
		{
			mKeyStats[C_LK] = true;
			mWasReleased[C_LK] = false;
		}
	}
	else
	{
		mKeyStats[C_LK] = false;
		mWasReleased[C_LK] = true;
	}


	if(pType&C_HK)
	{
		if(!mKeyStats[C_HK])
		{
			lPushedKey |= C_HK;
		}
		
		if(mWasReleased[C_HK])
		{
			mKeyStats[C_HK] = true;
			mWasReleased[C_HK] = false;
		}
	}
	else
	{
		mKeyStats[C_HK] = false;
		mWasReleased[C_HK] = true;
	}

	//****

	if(pType&BACK)
	{
		if(!mKeyStats[BACK])
		{
			lPushedKey |= BACK;
		}
		
		if(mWasReleased[BACK])
		{
			mKeyStats[BACK] = true;
			mWasReleased[BACK] = false;
		}
	}
	else
	{
		mKeyStats[BACK] = false;
		mWasReleased[BACK] = true;
	}

	//****

	if(pType&START)
	{
		if(!mKeyStats[START])
		{
			lPushedKey |= START;
		}
		
		if(mWasReleased[START])
		{
			mKeyStats[START] = true;
			mWasReleased[START] = false;
		}
	}
	else
	{
		mKeyStats[START] = false;
		mWasReleased[START] = true;
	}


	if(lPushedKey != 0x0)
	{
		mCommandBuffer.push_back((InputType)lPushedKey);
		mTTLCommand.push_back(0.5f);
	}
}

