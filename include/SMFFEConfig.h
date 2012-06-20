#ifndef SMFFE_SMFFECONFIG_H
#define SMFFE_SMFFECONFIG_H

#include <fstream>
#include <string>
#include "IniFile.h"

#include <SFML/Window.hpp>

#include "CommandInput.h"

class SMFFEConfig
{
protected:
	static SMFFEConfig sInstance;

	int		mWinWidth;
	int		mWinHeight;
	bool	mFullScreen;

	int		mViewWidth;
	int		mViewHeight;

	std::map<InputType, int> mKeyBoardMapping;
	std::map<InputType, int> mJoypadMapping[8];

	std::map<sf::String, sf::String> mDatas;

	std::string mFilename;

	float mPurcentageDamageTakenUltra;
	float mPurcentageDamageGivenUltra;
	float mPurcentageDamageTakenUltraFromUltra;

	SMFFEConfig(void);
	~SMFFEConfig(void);

public:
	static SMFFEConfig& instance() { return sInstance; }

	int getWinWidth() { return mWinWidth; }
	int getWinHeight() { return mWinHeight; }

	int getViewWidth() { return mViewWidth; }
	int getViewHeight() {return mViewHeight; }

	bool isFullscreen() { return mFullScreen; }
	void setFullscreen(bool pFullscreen) { mFullScreen = pFullscreen; }

	float getPurcentageGivenUltra() { return mPurcentageDamageGivenUltra; }
	float getPurcentageTakenUltra() { return mPurcentageDamageTakenUltra; }
	float getPurcentageTakenUltraFromUltra() { return mPurcentageDamageTakenUltraFromUltra; }

	std::map<sf::String, sf::String>& getDatas() { return mDatas; }

	void save();

	std::map<InputType, int>& getKeyboardMapping() { return mKeyBoardMapping; }
	std::map<InputType, int>& getJoypadMapping(int i) { return mJoypadMapping[i]; }

	std::map<InputType, int>& getMappingForInput(CommandInput* pInput);

	void setKeyboardMapping(std::map<InputType, int>& pMapping) { mKeyBoardMapping = pMapping; }
	void setJoypaddMapping(int pIndex, std::map<InputType, int>& pMapping) { mJoypadMapping[pIndex] = pMapping; }

	void saveKeyboardMapping();
	void saveJoypadMapping();

	void readKeyboardMapping();
	void readJoypadMapping();

//***** a BIT dirty, but stock here the value of different Player choosed thing for loading screen

	static sf::String	sChar1;
	static sf::String	sChar2;
	static bool			sCasu1;
	static bool			sCasu2;
	static sf::String	sArene;
	static bool			sTrainingMode;

	static bool			mMusicOn;
	static float		mEffectRatio;
};

#endif
