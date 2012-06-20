#ifndef SMFFE_COMBO_H
#define SMFFE_COMBO_H

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "CommandInput.h"

class Character;

class Combo
{
private:
	int mPrerequisedStance;
	std::list<InputType> mListKey;

	Character* mCharacter;

	float mTimeSinceStart;

	int mZoneTouched;

	sf::String mAnim;

	sf::SoundBuffer mBuffer;
	sf::SoundBuffer mBufferExecute;

	sf::Sound		mSoundExecute;
	sf::Sound		mSound;


	int mPreHit;
	int mActive;
	int mRecup;

	int mID;
	std::set<int> mInteruptID;

	float mStunTime;

	float mDamage;
	bool mHaveHit;

	bool mIsUltra;

	bool mProjectileSpawner;

public:
	Combo(const std::list<InputType> pListKey, sf::String pAnim, Character* pChar, int pPreHit, int pActive, int pRecup, float pDamage, int pZone, float pStunTime, int pPrerequis = NONE, bool pProjectileSpawner = false, bool pIsUltra = false);
	~Combo(void);

	enum COMBO_STATE{PREHIT, ACTIVE, RECUP};
	enum ZONE{A = 0x00000001, B = 0x00000002, C = 0x00000004};

	bool testAgainst(std::list<InputType> pCommandBuffer, InputType pCurrentStance, bool pInversed);

	void execute() { mHaveHit = false; mTimeSinceStart = 0; mSoundExecute.Play();}

	void hited() { mHaveHit = true; mSound.Play();}
	bool isHited() { return mHaveHit; }

	void setID(int pID) { mID = pID; }
	void addIDInterupted(int pID) { mInteruptID.insert(pID); }

	sf::String getAnim() { return mAnim; }

	int getZoneTouched() { return mZoneTouched; }
	float getDamage() { return mDamage; }
	float getStunTime() { return mStunTime; }

	bool spawnProjectile() { return mProjectileSpawner; }
	bool isUltra() { return mIsUltra; }

	int getID() { return mID; }
	bool isInteruptedBy(int pID);

	void setSound(const sf::String& pPath);
	void setSoundExecute(const sf::String& pPath);

	std::list<InputType>& getListKey() { return mListKey; }

	COMBO_STATE getCurrentState(int pFrame);

};

#endif

