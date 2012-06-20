#include "..\include\SMFFEConfig.h"
#include "helpers.h"

#include "KeyboardInput.h"
#include "JoypadInput.h"

SMFFEConfig SMFFEConfig::sInstance;

sf::String	SMFFEConfig::sChar1 = "";
sf::String	SMFFEConfig::sChar2 = "";
bool		SMFFEConfig::sCasu1 = true;
bool		SMFFEConfig::sCasu2 = true;
sf::String	SMFFEConfig::sArene = "";
bool		SMFFEConfig::sTrainingMode = false;

bool		SMFFEConfig::mMusicOn = true;
float		SMFFEConfig::mEffectRatio = 1.0f;

SMFFEConfig::SMFFEConfig(void)
	:mFilename("data/config.ini")
{
	mViewWidth = 1024;
	mViewHeight = 728;

	// ******* INI READING ****
	if(!fexists(mFilename.c_str()))
	{
		CIniFile::Create(mFilename);
	}

	//----width
	std::string lValue = CIniFile::GetValue("width", "graphics", mFilename);
	if(lValue == "")
	{
		mWinWidth = 1024;
	}
	else
	{
		mWinWidth = atoi(lValue.c_str());
	}

	//-----height
	lValue = CIniFile::GetValue("height", "graphics", mFilename);
	if(lValue == "")
	{
		mWinHeight = 768;
	}
	else
	{
		mWinHeight = atoi(lValue.c_str());
	}

	//-----fullscreen
	lValue = CIniFile::GetValue("fullscreen", "graphics", mFilename);
	if(lValue == "")
	{
		mFullScreen = false;
	}
	else
	{
		mFullScreen = lValue == "true"?true:false;
	}

	//-----sounds

	lValue = CIniFile::GetValue("music", "sounds", mFilename);
	if(lValue == "")
	{
		mMusicOn = true;
	}
	else
	{
		mMusicOn = lValue == "on"?true:false;
	}


	lValue = CIniFile::GetValue("effect", "sounds", mFilename);
	if(lValue == "")
	{
		mEffectRatio = 1.0f;
	}
	else
	{
		mEffectRatio = lValue == "on"?1.0f:0.0f;
	}


	//----------------------READ KEYBOARD/joypad MAPPING
	readKeyboardMapping();
	readJoypadMapping();

	//----------------------READ GLOBAL VARIABLES

	mDatas = parseDataFile("data/variables.conf");

	mPurcentageDamageTakenUltra = stringToFloat(mDatas["ultraReceivePurcentage"]);
	mPurcentageDamageGivenUltra = stringToFloat(mDatas["ultraDegatPurcentage"]);
}


SMFFEConfig::~SMFFEConfig(void)
{
	save();
}

//----------------------------------------------

void SMFFEConfig::save()
{
	CIniFile::SetValue("width", intToString(mWinWidth), "graphics", mFilename);
	CIniFile::SetValue("height", intToString(mWinHeight), "graphics", mFilename);
	CIniFile::SetValue("fullscreen", mFullScreen?"true":"false", "graphics", mFilename);

	CIniFile::SetValue("music", mMusicOn?"on":"off", "sounds", mFilename);

	saveKeyboardMapping();
	saveJoypadMapping();

	CIniFile::Sort(mFilename, true);
}

//----------------------------------------------

void SMFFEConfig::readKeyboardMapping()
{
	std::string lValue;

	lValue = CIniFile::GetValue("LP", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[C_LP] = sf::Keyboard::E;
	}
	else
	{
		mKeyBoardMapping[C_LP] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************

	lValue = CIniFile::GetValue("HP", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[C_HP] = sf::Keyboard::R;
	}
	else
	{
		mKeyBoardMapping[C_HP] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************

	lValue = CIniFile::GetValue("LK", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[C_LK] = sf::Keyboard::D;
	}
	else
	{
		mKeyBoardMapping[C_LK] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************

	lValue = CIniFile::GetValue("HK", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[C_HK] = sf::Keyboard::F;
	}
	else
	{
		mKeyBoardMapping[C_HK] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************

	lValue = CIniFile::GetValue("START", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[START] = sf::Keyboard::Return;
	}
	else
	{
		mKeyBoardMapping[START] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************

	lValue = CIniFile::GetValue("BACK", "keyboard", mFilename);
	if(lValue == "")
	{
		mKeyBoardMapping[BACK] = sf::Keyboard::Escape;
	}
	else
	{
		mKeyBoardMapping[BACK] = (sf::Keyboard::Key)atoi(lValue.c_str());
	}

	//************
}

//-----------------------------------------------------------

void SMFFEConfig::readJoypadMapping()
{
	for(int i = 0; i < 8; i++)
	{
		std::string lValue;

		lValue = CIniFile::GetValue("LP", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][C_LP] = 0;
		}
		else
		{
			mJoypadMapping[i][C_LP] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}

		//************

		lValue = CIniFile::GetValue("HP", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][C_HP] = 1;
		}
		else
		{
			mJoypadMapping[i][C_HP] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}

		//************

		lValue = CIniFile::GetValue("LK", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][C_LK] = 2;
		}
		else
		{
			mJoypadMapping[i][C_LK] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}

		//************

		lValue = CIniFile::GetValue("HK", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][C_HK] = 3;
		}
		else
		{
			mJoypadMapping[i][C_HK] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}

		//************

		lValue = CIniFile::GetValue("START", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][START] = 5;
		}
		else
		{
			mJoypadMapping[i][START] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}

		//************

		lValue = CIniFile::GetValue("BACK", sf::String("joystick") + intToString(i), mFilename);
		if(lValue == "")
		{
			mJoypadMapping[i][BACK] = 6;
		}
		else
		{
			mJoypadMapping[i][BACK] = (sf::Keyboard::Key)atoi(lValue.c_str());
		}
	}
}

//-----------------------------------------------------------

void SMFFEConfig::saveKeyboardMapping()
{
	CIniFile::SetValue("LP", intToString(mKeyBoardMapping[C_LP]), "keyboard", mFilename);
	CIniFile::SetValue("HP", intToString(mKeyBoardMapping[C_HP]), "keyboard", mFilename);
	CIniFile::SetValue("LK", intToString(mKeyBoardMapping[C_LK]), "keyboard", mFilename);
	CIniFile::SetValue("HK", intToString(mKeyBoardMapping[C_HK]), "keyboard", mFilename);
	CIniFile::SetValue("START", intToString(mKeyBoardMapping[START]), "keyboard", mFilename);
	CIniFile::SetValue("BACK", intToString(mKeyBoardMapping[BACK]), "keyboard", mFilename);
}

//-----------------------------------------------------------

void SMFFEConfig::saveJoypadMapping()
{
	for(int i = 0; i < 8; i++)
	{
		CIniFile::SetValue("LP", intToString(mJoypadMapping[i][C_LP]), sf::String("joystick") + intToString(i), mFilename);
		CIniFile::SetValue("HP", intToString(mJoypadMapping[i][C_HP]), sf::String("joystick") + intToString(i), mFilename);
		CIniFile::SetValue("LK", intToString(mJoypadMapping[i][C_LK]), sf::String("joystick") + intToString(i), mFilename);
		CIniFile::SetValue("HK", intToString(mJoypadMapping[i][C_HK]), sf::String("joystick") + intToString(i), mFilename);
		CIniFile::SetValue("START", intToString(mJoypadMapping[i][START]), sf::String("joystick") + intToString(i), mFilename);
		CIniFile::SetValue("BACK", intToString(mJoypadMapping[i][BACK]), sf::String("joystick") + intToString(i), mFilename);
	}
}

//-----------------------------------------------------------

std::map<InputType, int>& SMFFEConfig::getMappingForInput(CommandInput* pInput)
{
	KeyboardInput* lkeyboard = dynamic_cast<KeyboardInput*>(pInput);

	if(lkeyboard != NULL)
	{
		return getKeyboardMapping();
	}

	JoypadInput* ljoypad = dynamic_cast<JoypadInput*>(pInput);

	if(ljoypad != NULL)
	{
		return getJoypadMapping(ljoypad->getNumber());
	}
}