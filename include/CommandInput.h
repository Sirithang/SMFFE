#ifndef SMFFE_COMMAND_INPUT
#define SMFFE_COMMAND_INPUT

#include <list>
#include <map>

#include <SFML/System.hpp>

enum InputType
{
	NONE		= 0x00000000,
	UP			= 0x00000001,
	DOWN		= 0x00000002,
	RIGHT		= 0x00000004,
	LEFT		= 0x00000008,
	DOWNRIGHT	= 0x00000010,
	DOWNLEFT	= 0x00000020,
	UPRIGHT		= 0x00000040,
	UPLEFT		= 0x00000080,
	C_HP		= 0x00000100,
	C_HK		= 0x00000200,
	C_LP		= 0x00000400,
	C_LK		= 0x00000800,
	START		= 0x00001000,
	BACK		= 0x00002000
};

class CommandInput
{
protected:
	InputType mLastDirection;
	std::list<InputType>	mCommandBuffer;
	std::list<float>		mTTLCommand;

	std::map<InputType, bool> mWasReleased;
	std::map<InputType, bool> mKeyStats;

	void decrementeTTL();

	sf::String mType;

	virtual void pollImplementation() = 0;
	
	void pushed(InputType pType);

	int mNumber;

public:
	CommandInput();

	//-------- get/set--------------

	std::list<InputType>&	getCommandBuffer() { return mCommandBuffer; }
	InputType				getLastDirection() const { return mLastDirection; }

	void clearCommandBuffer() { mCommandBuffer.clear(); mTTLCommand.clear();}

	//set the input down at false after polling with this function.
	//Allow to get the input only once (no auto-fire)
	bool getInputDown(InputType pType);

	//------------------------------

	virtual void pollData();

	//-----------------------------

	int getNumber() { return mNumber; }
	sf::String getType() { return mType; }

};

#endif

