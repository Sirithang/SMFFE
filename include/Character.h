#ifndef SMFFE_CHARACTER_H
#define SMFFE_CHARACTER_H


#include <SFML/Graphics.hpp>
#include <vector>

#include "AnimatedSprite.h"
#include "Projectile.h"
#include "Combo.h"

//#include "Player.h"
//----------------------------------------------
// Class du personnage qu'incarnera le player
//----------------------------------------------

class Player;

class Character : public AnimatedSprite
{
protected:
	static sf::String sContactAnim;

	float			mPV;
	float			mPvMax;
	Sprite			mLifeUpperJaugeSprite;
	Sprite			mLifeSprite;
	sf::Vector2f	mLifeSpritePos;

	float			mUltraJauge;
	float			mUltraJaugeMax;
	Sprite			mUltraUpperJaugeSprite;
	Sprite			mUltraJaugeSprite;
	sf::Vector2f	mUltraJaugeSpritePos;
	Combo*			mUltraCombo;
	bool			mUltraComboLaunched;
	float			mUltraTimeSinceLastInput;
	std::list<InputType>::iterator mComboIt;

	float			mGuard;
	float			mGuardMax;
	Sprite			mGuardSprite;
	float			mTimerBeforeGuardAgain;

	Sprite*			mPortrait;

	Player*			mPlayer;
	float			mHeight;
	Character*		mOther;
	sf::String		mFolder;
	

	AnimatedSprite*	mUltraButtons[4]; // LP, HP, LK, HK
	AnimatedSprite* mUltraButtonDisplayed;

	std::vector<Combo*> mCombos;
	std::list<Projectile*> mProjectiles;
	sf::Vector2f mPosition;
	Combo* mCurrentCombo;

	std::vector<Sprite*> mAssociatedSprite;

	int				mNumberRoundWin;

	

	float			mSpeed;
	sf::Vector2f	mVelocity;
	bool			mGrounded;
	sf::Vector2f	mLastPosition;

	sf::Vector2f	mProjectileSpawn; // relative to upper-corner

	

	bool			mHit;
	float			mStunHit;

	bool			mBlocking;
	bool			mCrouching;

	bool			mCanControl;

	bool			mGrosCasuMode;

	
	std::map<sf::String, sf::String> mDatas; // datas read from data file

	// ***** Ultra stuff 

	bool		mUltra;
	sf::String	mUltraAnim;
	int			mUltraNbHit;
	int			mUltraNbHitDone;
	float		mUltraTimeBetweenHit;
	float		mUltraTimeSinceLastHit;
	int			mUltraDegatByHit;

	bool		mTrainingMode;

	//***************

	void velocityFromInput();
	void animationFromInput();

	void loadCombos();
	void loadUltra();

	void checkCombo();

	void checkAssociatedSprites();
	void chooseSpriteToDisplay(const InputType& pInput);

public:
	Character(const sf::Vector2f pPosition, Player* pPlayer, bool pCasu);
	~Character(void);

	void init();

	void setTrainingMode(bool ptraining) { mTrainingMode = ptraining; }

	void loadFromFolder(sf::String pFolder);

	void setOtherCharacter(Character* pOther) { mOther = pOther; }

	void AddCombo(Combo* pCombo) { mCombos.push_back(pCombo); }

	void update();
	void draw(sf::RenderTarget* pTarget);

	void madeContact();
	void takeDamage(float pAmount, float pStunTime);

	void guard();

	void spawnProjectile(sf::Vector2f pDirection);
	void updateProjectiles();
	std::list<Projectile*>& getProjectiles() { return mProjectiles; }

	void checkCollisionsWithProjectiles(std::list<Projectile*> &pProjectile);

	//Useful for collision handling (made after the two char have moved)
	void lateUpdate();

	void reloadInfo() { stopAll(); loadFromFolder(mFolder); }

	void setCanControl(bool pControl) { mCanControl = pControl; }

	std::vector<Sprite*>& getAssociatedSprite() { return mAssociatedSprite; }

	bool isDead() { return mPV <= 0; }
	void reset();

	void die() {mCanControl = false; play("defeat"); }

	bool isInUltra() { return mUltra; }

	void winARound() { mNumberRoundWin++; mCanControl = false; play("win");}
	int  getNumberOfRoundWon() { return mNumberRoundWin; }

	float getLifePurcentage() { return (float)mPV/(float)mPvMax; }


	float	mPurcentageLoaded;
};
#endif