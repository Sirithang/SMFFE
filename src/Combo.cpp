#include "..\include\Combo.h"

using namespace std;

Combo::Combo(const std::list<InputType> pListKey, sf::String pAnim, Character* pChar, int pPreHit, int pActive, int pRecup, float pDamage, int pZone, float pStunTime, int pPrerequis, bool pProjectileSpawner, bool pIsUltra)
	:mListKey(pListKey) , mAnim(pAnim), mCharacter(pChar), mPreHit(pPreHit), mActive(pActive), 
	mRecup(pRecup), mDamage(pDamage), mStunTime(pStunTime), mPrerequisedStance(pPrerequis), mZoneTouched(pZone), mProjectileSpawner(pProjectileSpawner),
	mIsUltra(pIsUltra)
{
	mHaveHit = false;
}


Combo::~Combo(void)
{
}

//--------------------------------------------------

void Combo::setSound(const sf::String& pPath)
{
	mBuffer.LoadFromFile(pPath);
	mSound.SetBuffer(mBuffer);
}

void Combo::setSoundExecute(const sf::String& pPath)
{
	mBufferExecute.LoadFromFile(pPath);
	mSoundExecute.SetBuffer(mBufferExecute);
}

//--------------------------------------------------

bool Combo::testAgainst(list<InputType> pCommandBuffer, InputType pCurrentStance, bool pInversed)
{
	if(mPrerequisedStance != NONE && (pCurrentStance & mPrerequisedStance) == 0)
		return false;

	if(pCommandBuffer.size() < mListKey.size())
		return false;

	list<InputType>::iterator lItBuffer = pCommandBuffer.end();
	list<InputType>::iterator lItKey = mListKey.end();

	while(lItBuffer != pCommandBuffer.begin() && lItKey != mListKey.begin())
	{
		lItBuffer--;
		lItKey--;

		InputType lTru3key = (*lItBuffer);

		if(pInversed)
		{
			switch(lTru3key)
			{
			case LEFT:
				lTru3key = RIGHT;
				break;
			case RIGHT:
				lTru3key = LEFT;
				break;
			case UPLEFT:
				lTru3key = UPRIGHT;
				break;
			case UPRIGHT:
				lTru3key = UPLEFT;
				break;
			case DOWNLEFT:
				lTru3key = DOWNRIGHT;
				break;
			case DOWNRIGHT:
				lTru3key = DOWNLEFT;
				break;
			default:
				break;
			}
		}

		int result = lTru3key&(*lItKey);
		if(result == 0)
			return false;
	}

	return true;

}

//-----------------------------------------------------

Combo::COMBO_STATE Combo::getCurrentState(int pFrame)
{
	if(pFrame < mPreHit)
		return PREHIT;

	if(pFrame < mPreHit + mActive)
		return ACTIVE;

	
	return RECUP;
}

//------------------------------------------------------

bool Combo::isInteruptedBy(int pID)
{
	return mInteruptID.count(pID) > 0;
}