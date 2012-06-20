#include "Character.h"
#include "constants.h"
#include "helpers.h"

#include "SMFFETime.h"
#include "SMFFEConfig.h"
#include "JoypadInput.h"

#include "Player.h"
#include "TimedSprite.h"

#include <iostream>

sf::String Character::sContactAnim = "data/sprites/particules/particuleshit";


Character::Character(const sf::Vector2f pPosition, Player* pPlayer, bool pCasu)
	:	AnimatedSprite(pPosition), 
		mLifeUpperJaugeSprite("data/GUI/HUD/Barre de vie vide.png", sf::Vector2f(0,0)),
		mLifeSprite("data/GUI/HUD/Barre de vie pleine.png", sf::Vector2f(0,0)),
		mUltraUpperJaugeSprite("data/GUI/HUD/Barre d'ultra vide.png", sf::Vector2f(0,0)),
		mUltraJaugeSprite("data/GUI/HUD/Barre d'ultra pleine.png", sf::Vector2f(0,0)),
		mGuardSprite("data/GUI/HUD/Barre de garde.png", sf::Vector2f(0,0)),
		mGrosCasuMode(pCasu)
{
	mPurcentageLoaded = 0;
	
	mPlayer = pPlayer;
	mSpeed = 100.0f;

	mGrounded = false;

	mCurrentCombo = NULL;
	mUltraButtonDisplayed = NULL;

	mHit = false;
	mBlocking = false;
	mCrouching = false;

	mCanControl = true;

	mHeight = 80;

	mNumberRoundWin = 0;

	mProjectileSpawn = sf::Vector2f(50, 10);

	mFolder = "";

	mUltra = false;
	mUltraJauge = 0;
	mUltraJaugeMax = stringToFloat(SMFFEConfig::instance().getDatas()["ultraJauge"]);

	mUltraComboLaunched = false;

	mTrainingMode = false;

	//mUltraJaugeSprite.getSFSprite().SetColor(sf::Color::Blue);

	if(!mPlayer->isLeftSide())
	{
		mLifeSpritePos = sf::Vector2f(SMFFEConfig::instance().getViewWidth() - mLifeSprite.getSFSprite().GetSize().x, 0);
		mLifeSprite.flipX(true);
		mLifeUpperJaugeSprite.flipX(true);

		mUltraJaugeSpritePos = sf::Vector2f(SMFFEConfig::instance().getViewWidth() - mUltraJaugeSprite.getSFSprite().GetSize().x,
											SMFFEConfig::instance().getViewHeight() - mUltraJaugeSprite.getSFSprite().GetSize().y);
		mUltraJaugeSprite.flipX(true);
		mUltraUpperJaugeSprite.flipX(true);

		mGuardSprite.flipX(true);
		//mUltraJaugeSprite.getSFSprite().FlipY(true);
	}
	else
	{
		mLifeSpritePos = sf::Vector2f(0,0);

		mUltraJaugeSpritePos = sf::Vector2f(0,
											SMFFEConfig::instance().getViewHeight() - mUltraJaugeSprite.getSFSprite().GetSize().y);
		
		//mUltraJaugeSprite.getSFSprite().FlipY(true);
	}


	///*********qte image loading

	sf::Vector2f lButtonPos;
	int buttonsize = 40;
	if(!mPlayer->isLeftSide())
	{
		lButtonPos = sf::Vector2f(	SMFFEConfig::instance().getViewWidth() - 150 - buttonsize,
									SMFFEConfig::instance().getViewHeight() - 100 - buttonsize);
	}
	else
	{
		lButtonPos = sf::Vector2f(	150 + buttonsize,
									SMFFEConfig::instance().getViewHeight() - 100 - buttonsize);
	}

	mUltraButtons[0] = new AnimatedSprite(lButtonPos);
	mUltraButtons[0]->addAnimation("idle", "data/GUI/QTE/lp", 0.2f);
	mUltraButtons[0]->play("idle");

	mUltraButtons[1] = new AnimatedSprite(lButtonPos);
	mUltraButtons[1]->addAnimation("idle", "data/GUI/QTE/hp", 0.2f);
	mUltraButtons[1]->play("idle");

	mUltraButtons[2] = new AnimatedSprite(lButtonPos);
	mUltraButtons[2]->addAnimation("idle", "data/GUI/QTE/lk", 0.2f);
	mUltraButtons[2]->play("idle");

	mUltraButtons[3] = new AnimatedSprite(lButtonPos);
	mUltraButtons[3]->addAnimation("idle", "data/GUI/QTE/hk", 0.2f);
	mUltraButtons[3]->play("idle");

	mGuardMax = stringToFloat(SMFFEConfig::instance().getDatas()["ultraJauge"]);
	mGuard = mGuardMax;
	
	mTimerBeforeGuardAgain = 0;

	mNumberRoundWin = 0;

}

//-----------------------------------------------------

Character::~Character(void)
{
	delete mPortrait;
}

void Character::update()
{

	if(mTrainingMode)
	{
		mUltraJauge = std::min(mUltraJauge + Time::deltaTime() * 50, mUltraJaugeMax);
		mPV = std::min(mPV + Time::deltaTime() * 50, mPvMax);
	}

	if(mTimerBeforeGuardAgain > 0)
		mTimerBeforeGuardAgain -= Time::deltaTime();

	mPlayer->getInput()->pollData();
	
	if(mUltra)
	{
		mUltraTimeSinceLastHit += Time::deltaTime();

		while(mUltraTimeSinceLastHit > mUltraTimeBetweenHit)
		{
			mOther->takeDamage(mUltraDegatByHit, 1.0f);
			mUltraTimeSinceLastHit -= mUltraTimeBetweenHit;
			mUltraNbHitDone++;
		}

		if(mUltraNbHitDone == mUltraNbHit)
		{
			mUltra = false;
			mOther->mCanControl = true;
		}
	}
	else if(mCurrentCombo == NULL && !mHit && !mBlocking && mGrounded && mCanControl)
	{
		velocityFromInput();
		animationFromInput();

		checkCombo();

		if(mTimerBeforeGuardAgain < 0.001f)
		{
			mGuard = std::min(mGuard + Time::deltaTime(), mGuardMax);
		}


		// FLIP CHECK

		float otherLeft = mOther->getPosition().x;
		float left = getPosition().x;
		if(mWasFliped)
			left -= getSFSprite().GetSize().x - 50;

		if(mOther->mWasFliped)
			otherLeft -= mOther->getSFSprite().GetSize().x - 50;

		if(otherLeft < 
			left)
		{
			if(!mWasFliped)
			{
				flipX(true);
			}
		}
		else
		{
			if(mWasFliped)
			{
				flipX(false);
			}
		}
	}
	else if(mHit)
	{
		mStunHit -= Time::deltaTime();
		if(mStunHit <= 0)
			mHit = false;
	}
	else if (mBlocking)
	{
		mVelocity.x = 0;
	}
	else if(mCurrentCombo != NULL)
	{
		mVelocity.x = 0;
		if(getCurrentAnimation()->finished())
		{
			mCurrentCombo = NULL;
		}
	}
	else
	{
		// FLIP CHECK
		if(mGrounded)
		{
			float otherLeft = mOther->getPosition().x;
			float left = getPosition().x;
			if(mWasFliped)
				left -= getSFSprite().GetSize().x - 50;

			if(mOther->mWasFliped)
				left -= mOther->getSFSprite().GetSize().x - 50;

			if(otherLeft < 
				left)
			{
				if(!mWasFliped)
				{
					flipX(true);
				}
			}
			else
			{
				if(mWasFliped)
				{
					flipX(false);
				}
			}
		}

		animationFromInput();
	}

	//"gravity"
	if(!mGrounded)
		mVelocity.y += 30 * Time::deltaTime();

	//collision
	if(mLastCollisionResult.mCollided)
	{
		//mVelocity.x = 0;
	}

	mLastPosition = getPosition();
	setPosition(getPosition() + mVelocity);

	if(getPosition().y + mSprite->GetSize().y >= 650)
	{
		mGrounded = true;
		setPosition(sf::Vector2f(getPosition().x, 650 - mSprite->GetSize().y));
		mVelocity.y = 0;
	}
	else
	{
		mGrounded = false;
	}

	//--------side stop-------------------

	float lLeft;
	if(mWasFliped)
	{
		lLeft = SMFFEConfig::instance().getViewWidth();
	}
	else
	{
		lLeft = SMFFEConfig::instance().getViewWidth() - mSprite->GetSize().x;
	}

	if(getPosition().x < 0)
		setPosition(sf::Vector2f(0, mSprite->GetPosition().y));

	if(getPosition().x > lLeft)
		setPosition(sf::Vector2f(lLeft, mSprite->GetPosition().y));

	//------------------------------------

	updateProjectiles();
	checkAssociatedSprites();

	AnimatedSprite::update();
}

//---------------------------------------------------

void Character::chooseSpriteToDisplay(const InputType& pInput)
{
	switch(pInput)
	{
	case C_LP:
		mUltraButtonDisplayed = mUltraButtons[0];
		break;
	case C_HP:
		mUltraButtonDisplayed = mUltraButtons[1];
		break;
	case C_LK:
		mUltraButtonDisplayed = mUltraButtons[2];
		break;
	case C_HK:
		mUltraButtonDisplayed = mUltraButtons[3];
		break;
	default:
		mUltraButtonDisplayed = NULL;
		break;
	}
}

//----------------------------------------------------

void Character::checkCombo()
{
	if(mGrosCasuMode && (mUltraJauge == mUltraJaugeMax))
	{
		chooseSpriteToDisplay(mUltraCombo->getListKey().front());
		if(!mUltraComboLaunched)
		{//combo not launched
			if(	mPlayer->getInput()->getInputDown(mUltraCombo->getListKey().front()))
			{//if input == 1st key of combo
				mUltraComboLaunched = true;
				mUltraTimeSinceLastInput = 3.0f;
				mComboIt = mUltraCombo->getListKey().begin();
				mComboIt++;
			}
		}
	}

	if(mUltraComboLaunched)
	{//combo laucnehd
		if(mComboIt == mUltraCombo->getListKey().end())
		{//we reach the end of combo, done, validate
			mPlayer->getInput()->clearCommandBuffer();

			mUltra = true;
			mUltraJauge = 0;
			mUltraNbHitDone = 0;
			mUltraTimeSinceLastHit = 0;
			play(mUltraAnim, true);

			mUltraComboLaunched = false;

			mUltraCombo->execute();

			mOther->mCanControl = false;

			mVelocity.x = 0;

			chooseSpriteToDisplay(UP);//just to force default

			return;
		}

		chooseSpriteToDisplay(*mComboIt);

		mUltraTimeSinceLastInput -= Time::deltaTime();
		if(mUltraTimeSinceLastInput < 0)
		{//we wait to long before Input
			mUltraComboLaunched = false;
			chooseSpriteToDisplay(UP);
			mUltraJauge = 0;
		}
		else
		{
			if(mPlayer->getInput()->getInputDown(*mComboIt))
			{//next touch pressed
				mComboIt++;
				mUltraTimeSinceLastHit = 3.0f;
				return;
			}
		}
	}

	for(int i = 0; i < mCombos.size(); i++)
	{
		if(mCombos.at(i)->testAgainst(mPlayer->getInput()->getCommandBuffer(), mPlayer->getInput()->getLastDirection(), mWasFliped))
		{
			mPlayer->getInput()->clearCommandBuffer();

			if(mCombos.at(i)->isUltra())
			{
				if(mUltraJauge == mUltraJaugeMax)
				{
					mUltra = true;
					mUltraJauge = 0;
					mUltraNbHitDone = 0;
					mUltraTimeSinceLastHit = 0;
					play(mUltraAnim, true);

					mUltraCombo->execute();

					mOther->mCanControl = false;

					mVelocity.x = 0;
				}
			}
			else
			{
				if(mCurrentCombo == NULL)
				{
					mCurrentCombo = mCombos.at(i);
					mCurrentCombo->execute();
					play(mCurrentCombo->getAnim(), false);

					if(mCurrentCombo->spawnProjectile())
					{
						sf::Vector2f pDir(1, 0);
						if(mWasFliped)
							pDir.x *= -1;

						spawnProjectile(pDir);
					}
				}
				else
				{
					if(mCurrentCombo->isInteruptedBy(mCombos.at(i)->getID()))
					{
						mCurrentCombo = mCombos.at(i);
						mCurrentCombo->execute();
						play(mCurrentCombo->getAnim(), true);
					}
				}
			}

			break;
		}
	}
}

//---------------------------------------------------

void Character::checkAssociatedSprites()
{
	std::vector<Sprite*>::iterator it = mAssociatedSprite.begin(); 

	while(it != mAssociatedSprite.end())
	{
		(*it)->update();
		if((*it)->isToDelete())
		{
			delete *it;
			it = mAssociatedSprite.erase(it);
		}
		else
			it++;
	}
}

//---------------------------------------------------

void Character::lateUpdate()
{

	//****************** CHECKING FOR GUARD ****************//

	/*if(!mCanControl || mUltra)
		return;*/

	mBlocking = false;

	if(mOther->mCurrentCombo != NULL && !mHit && mCanControl)
	{
		guard();
	}

	if(mLastCollisionResult.mCollided)
	{
		//if(getPosition() != mLastPosition)
		setPosition(getPosition() + sf::Vector2f(mLastCollisionResult.mPenetrationFactor.x * (mWasFliped?1.0f:-1.0f), 0));
	}

	if(mLastCollisionResult.mHit && mOther->mCurrentCombo != NULL && !mOther->mCurrentCombo->isHited())
	{
		bool mActuallyHit = true;

		if(mBlocking && mGuard > 0.1f)
		{
			mActuallyHit = false;
			if(!mCrouching)
			{
				if((mOther->mCurrentCombo->getZoneTouched() & Combo::C) != 0)
				{
					mActuallyHit = true;
				}
			}
			else
			{
				if((mOther->mCurrentCombo->getZoneTouched() & Combo::A) != 0)
				{
					mActuallyHit = true;
				}
			}
		}

		mOther->madeContact();
		if(mActuallyHit)
		{
			takeDamage(mOther->mCurrentCombo->getDamage(), mOther->mCurrentCombo->getStunTime());
		}
		else
		{
			mGuard -= 1;
			if(mGuard < 0.01f)
			{
				mTimerBeforeGuardAgain = 5.0f;
				mOther->mUltraJauge = std::min(mOther->mUltraJauge + stringToFloat(SMFFEConfig::instance().getDatas()["ultraGuardBreak"]), mUltraJaugeMax);
			}
		}
	}
}

//---------------------------------------------------


void Character::velocityFromInput()
{
	if(mGrounded && mCurrentCombo == NULL)
	{
		InputType lLast = mPlayer->getInput()->getLastDirection();

		mVelocity.x = 0;

		mVelocity.x -= (lLast == LEFT ? 1.0f : 0.0f) * mSpeed * Time::deltaTime();
		mVelocity.x += (lLast == RIGHT ? 1.0f : 0.0f) * mSpeed * Time::deltaTime();

		mVelocity.x -= (lLast == UPLEFT ? 1.0f : 0.0f) * mSpeed * 2 * Time::deltaTime();
		mVelocity.x += (lLast == UPRIGHT ? 1.0f : 0.0f) * mSpeed * 2 * Time::deltaTime();

		mVelocity.y -= (lLast == UP || lLast == UPLEFT || lLast == UPRIGHT ? 1.0f : 0.0f) * 15;
	}

	
}

//--------------------------------------------------

void Character::animationFromInput()
{
	InputType lLast = mPlayer->getInput()->getLastDirection();

	mCrouching = false;

	switch(lLast)
	{
	case DOWN:
	case DOWNLEFT:
	case DOWNRIGHT:
		if(!mGrounded || !mCanControl) break;
		play("baisser");
		mCrouching = true;
		return;
		break;
	case LEFT:
	case RIGHT:
		if(!mGrounded || !mCanControl)break;
		play("walk");
		return;
		break;
	case UP:
	case UPLEFT:
	case UPRIGHT:
		if(!mGrounded || !mCanControl) break;
		play("jump");
		return;
		break;
	default:
		break;
	}

	if(mGrounded && mCanControl)
		play("idle");
}

//--------------------------------------------------------------------

void Character::spawnProjectile(sf::Vector2f pDirection)
{

	sf::Vector2f proj = mProjectileSpawn;

	if(mWasFliped)
	{
		proj.x -= (proj.x - mSprite->GetSubRect().Width * 0.5f) * 2.0f;
	}

	mProjectiles.push_back(new Projectile(getPosition() + proj * mSprite->GetScale().x, pDirection, 200));
	mProjectiles.back()->addAnimation("projectile", "data/debug/projectile", 0.2f);
	mProjectiles.back()->getSFSprite().SetScale(mSprite->GetScale());

	if(mWasFliped)
	{
		mProjectiles.back()->flipX(true);
	}

	mProjectiles.back()->play("projectile");
}

//---------------------------------------------------------------------

void Character::updateProjectiles()
{
	std::list<Projectile*>::iterator it = mProjectiles.begin() ;
	
	while(it != mProjectiles.end())
	{
		(*it)->update();

		if( (*it)->getPosition().x < -20 || (*it)->getPosition().x > 2*SMFFEConfig::instance().getWinWidth())
		{
			delete (*it);

			it = mProjectiles.erase(it);
		}
		else
			it++;
	}
}

//---------------------------------------------------

void Character::draw(sf::RenderTarget* pTarget)
{
	/*sf::Texture ltext;
	ltext.LoadFromImage(*mCollisionMask);
	sf::Sprite lspr(ltext);

	if(mWasFliped)
		lspr.SetPosition(getPosition() - sf::Vector2f(lspr.GetSize().x, 0));
	else
		lspr.SetPosition(getPosition());

	pTarget->Draw(lspr);

	if(!mWasFliped)
	{
		sf::Image* lImg = mLastCollisionResult.mMask;

		if(lImg != NULL)
		{
			sf::Texture lText2;
			lText2.LoadFromImage(*lImg);
			sf::Sprite lSpr2(lText2);

			lSpr2.SetPosition(400, 0);
			pTarget->Draw(lSpr2);
		}
	}*/

	AnimatedSprite::draw(pTarget);

	//******

	for(std::list<Projectile*>::iterator it = mProjectiles.begin();
		it != mProjectiles.end();
		it++)
	{
		(*it)->draw(pTarget);
	}

	//******* pv

	sf::IntRect lRect = mLifeSprite.getSFSprite().GetSubRect();
	int previouswidth = lRect.Width;

	lRect.Width = mLifeSprite.getSFSprite().GetTexture()->GetWidth() * ((float)mPV/(float)mPvMax);
	//if(!mPlayer->isLeftSide())
		//lRect.Left += previouswidth - lRect.Width;

	float lDiff = mLifeSprite.getSFSprite().GetTexture()->GetWidth() - lRect.Width;
	
	mLifeSprite.getSFSprite().SetSubRect(lRect);

	if(!mPlayer->isLeftSide())
	{
		mLifeSprite.setPosition(	sf::Vector2f(	SMFFEConfig::instance().getViewWidth() + lDiff ,0));
		mLifeUpperJaugeSprite.setPosition(sf::Vector2f(	SMFFEConfig::instance().getViewWidth(),0));
	}								
	else
	{
		mLifeSprite.setPosition(	sf::Vector2f(0,0) );
		mLifeUpperJaugeSprite.setPosition(mLifeSprite.getPosition());
	}

	//********* guard
	lRect = mGuardSprite.getSFSprite().GetSubRect();

	lRect.Width = mGuardSprite.getSFSprite().GetTexture()->GetWidth() * ((float)mGuard/(float)mGuardMax);
	
	lDiff = mGuardSprite.getSFSprite().GetTexture()->GetWidth() - lRect.Width;

	mGuardSprite.getSFSprite().SetSubRect(lRect);

	if(!mPlayer->isLeftSide())
	{
		mGuardSprite.setPosition(	sf::Vector2f(	SMFFEConfig::instance().getViewWidth() + lDiff ,0));
	}								
	else
	{
		mGuardSprite.setPosition(	sf::Vector2f(0,0) );
	}

	mGuardSprite.draw(pTarget);
	mPortrait->draw(pTarget);
	mLifeSprite.draw(pTarget);
	mLifeUpperJaugeSprite.draw(pTarget);

	//********* ultra

	lRect = mUltraJaugeSprite.getSFSprite().GetSubRect();

	lRect.Width = mUltraJaugeSprite.getSFSprite().GetTexture()->GetWidth() * ((float)mUltraJauge/(float)mUltraJaugeMax);
	
	lDiff = mUltraJaugeSprite.getSFSprite().GetTexture()->GetWidth() - lRect.Width;

	mUltraJaugeSprite.getSFSprite().SetSubRect(lRect);

	if(!mPlayer->isLeftSide())
	{
		mUltraJaugeSprite.setPosition(	sf::Vector2f(	SMFFEConfig::instance().getViewWidth() + lDiff,
														SMFFEConfig::instance().getViewHeight() - mUltraJaugeSprite.getSFSprite().GetSize().y));
		mUltraUpperJaugeSprite.setPosition(sf::Vector2f(	SMFFEConfig::instance().getViewWidth(),
														SMFFEConfig::instance().getViewHeight() - mUltraUpperJaugeSprite.getSFSprite().GetSize().y));
	}
	else
	{
		mUltraJaugeSprite.setPosition(	mUltraJaugeSpritePos );
		mUltraUpperJaugeSprite.setPosition(mUltraJaugeSpritePos);
	}

	
	mUltraJaugeSprite.draw(pTarget);
	mUltraUpperJaugeSprite.draw(pTarget);

	if(mUltraButtonDisplayed != NULL)
	{
		mUltraButtonDisplayed->update();
		mUltraButtonDisplayed->draw(pTarget);
	}


}

//----------------------------------------------------

void Character::checkCollisionsWithProjectiles(std::list<Projectile*>& pProjectile)
{
	std::list<Projectile*>::iterator it = pProjectile.begin();

	while(it != pProjectile.end())
	{
		if(mWasFliped)
		{
			if((*it)->getPosition().x < getPosition().x)
				guard();
		}
		else
		{
			if((*it)->getPosition().x > getPosition().x)
				guard();
		}

		if((*it)->testMaskAgainst(this, false))
		{
			if(!mHit && !mBlocking)
			{
				takeDamage(0, 0);
			}

			it = pProjectile.erase(it);
		}
		else
			it++;
	}
}

//----------------------------------------------------------

void Character::madeContact()
{
	if(mCurrentCombo != NULL) 
	{
		mCurrentCombo->hited();
		mUltraJauge = std::min(mUltraJauge + mCurrentCombo->getDamage() * SMFFEConfig::instance().getPurcentageGivenUltra(), mUltraJaugeMax);
	}
}

//----------------------------------------------------------

void Character::takeDamage(float pAmount, float pStunTime)
{
	mHit = true;
	play("hit", true, true);

	mPV -= pAmount;
	mStunHit = pStunTime;

	mUltraJauge = std::min(mUltraJauge + pAmount * SMFFEConfig::instance().getPurcentageTakenUltra(), mUltraJaugeMax);

	mAssociatedSprite.push_back(new TimedSprite(mLastCollisionResult.mPosition, 1.0f));
	/*mAssociatedSprite.back()->setPosition(	mLastCollisionResult.mPosition
		- sf::Vector2f(mAssociatedSprite.back()->getSFSprite().GetSubRect().Width * 20, mAssociatedSprite.back()->getSFSprite().GetSubRect().Height));*/
	static_cast<TimedSprite*>(mAssociatedSprite.back())->addAnimation("idle", sContactAnim, 1.0f);
	static_cast<TimedSprite*>(mAssociatedSprite.back())->play("idle");
}

//-----------------------------------------------------------

void Character::guard()
{
	if(mBlocking || mGuard < 0.01f || !mCanControl)
		return;

	InputType lBack;
	sf::String pAnim;

	if(mCrouching)
	{
		if(mWasFliped) lBack = DOWNRIGHT;
		else lBack = DOWNLEFT;
		pAnim = "crouchGuard";
	}
	else
	{
		if(mWasFliped) lBack = RIGHT;
		else lBack = LEFT;
		pAnim = "guard";
	}

	if(mPlayer->getInput()->getLastDirection() == lBack)
	{
		play(pAnim, false);
		mBlocking = true;
	}
}

//**************************************************************************

void Character::loadFromFolder(sf::String pFolder)
{
	if(mFolder == "")
	{
		mFolder = pFolder;
	}
	else
	{
		mAnimations.clear();
		mCombos.clear();
	}

	mPortrait = new Sprite(mFolder+"/portrait.png", sf::Vector2f(0,0));
	if(!mPlayer->isLeftSide())
	{
		mPortrait->flipX(true);
		mPortrait->setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth(),
							  0));
	}
	else
	{

	}

	// ******* loading default animation ******** ////

	mDatas.clear();
	mDatas = parseDataFile(mFolder+"/data.conf");

	if(mDatas.count("idle") > 0)
	{
		addAnimation("idle", mFolder+"/sprites/idle", stringToFloat(mDatas["idle"]));
	}
	else
	{
		toDebugLog(mFolder + " : IDLE TIME NOT DEFINED");
	}

	//--

	if(mDatas.count("walk") > 0)
	{
		addAnimation("walk", mFolder+"/sprites/walk", stringToFloat(mDatas["walk"]));
	}
	else
	{
		toDebugLog(mFolder + " : WALK TIME NOT DEFINED");
	}

	//--

	if(mDatas.count("jump") > 0)
	{
		addAnimation("jump", mFolder+"/sprites/jump", stringToFloat(mDatas["jump"]));
	}
	else
	{
		toDebugLog(mFolder + " : JUMP TIME NOT DEFINED");
	}

	//--

	if(mDatas.count("crouch") > 0)
	{
		addAnimation("baisser", mFolder+"/sprites/crouch", stringToFloat(mDatas["crouch"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : CROUCH TIME NOT DEFINED");
	}

	//--

	if(mDatas.count("hit") > 0)
	{
		addAnimation("hit", mFolder+"/sprites/hit", stringToFloat(mDatas["hit"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : HIT TIME NOT DEFINED");
	}

	if(mDatas.count("guard") > 0)
	{
		addAnimation("guard", mFolder+"/sprites/block", stringToFloat(mDatas["guard"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : GUARD TIME NOT DEFINED");
	}

	if(mDatas.count("crouchGuard") > 0)
	{
		addAnimation("crouchGuard", mFolder+"/sprites/blockCrouch", stringToFloat(mDatas["crouchGuard"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : CROUCH TIME NOT DEFINED");
	}

	if(mDatas.count("defeat") > 0)
	{
		addAnimation("defeat", mFolder+"/sprites/defeat", stringToFloat(mDatas["defeat"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : CROUCH TIME NOT DEFINED");
	}

	if(mDatas.count("win") > 0)
	{
		addAnimation("win", mFolder+"/sprites/win", stringToFloat(mDatas["win"]), false);
	}
	else
	{
		toDebugLog(mFolder + " : CROUCH TIME NOT DEFINED");
	}

	//******************* LOADING CHARA DATA*****************************

	if(mDatas.count("pv") > 0)
	{
		mPvMax = stringToFloat(mDatas["pv"]);
		mPV = mPvMax;
	}
	else
	{
		toDebugLog(mFolder + " : PV NOT DEFINED");
	}

	if(mDatas.count("speed") > 0)
	{
		mSpeed = stringToFloat(mDatas["speed"]);
	}

	if(mDatas.count("scale") > 0)
	{
		float lScale = stringToFloat(mDatas["scale"]);

		mSprite->SetScale(lScale, lScale);
	}

	mPurcentageLoaded = 0.5;

	loadUltra();
	loadCombos();

	mPurcentageLoaded = 1.0f;
}


//------------------------------------------------------------------------------------------------

void Character::loadCombos()
{
	std::vector<std::string> lFolders = getAllFolders(mFolder+"/combos");

	float lOriginalPurcentage = mPurcentageLoaded;

	for(int i = 0 ; i < lFolders.size(); i++)
	{
		if(lFolders.at(i) == "ultra" || lFolders.at(i) == "Ultra")
			continue;

		mPurcentageLoaded = lOriginalPurcentage + ((float)i/(float)lFolders.size()) * 0.5f;

		std::map<sf::String, sf::String> lData = parseDataFile(mFolder+"/combos/"+lFolders.at(i)+"/info.conf");

		if(lData.size() == 0)
			continue;

		try
		{
			sf::String lAnim = lData["animation"];
			float lSpeed = stringToFloat(lData["speed"]);
			
			int preHit = stringToFloat(lData["preHit"]);
			int active = stringToFloat(lData["active"]);
			int recup = stringToFloat(lData["recup"]);

			int damage = stringToFloat(lData["damage"]);

			//***zone parsing**//
			sf::String lZones = lData["zoneTouched"];
			std::vector<std::string> lZoneSplit = split(lZones.ToAnsiString(), '|');
			int lZone = 0;
			for(int  k = 0 ; k < lZoneSplit.size(); k++)
			{
				if(lZoneSplit.at(k) == "A")
					lZone |= Combo::A;
				else if(lZoneSplit.at(k) == "B")
					lZone |= Combo::B;
				else if(lZoneSplit.at(k) == "C")
					lZone |= Combo::C;
			}
			//*****************//

			float lStunTime = stringToFloat(lData["stunTime"]);


			//***combos parsing***//
			std::vector<std::string> lKeys = split(lData["combo"].ToAnsiString().substr(1, lData["combo"].ToAnsiString().size()-2), ',');
			std::list<InputType> lComboKey;

			for(int k = 0; k < lKeys.size(); k++)
			{
				InputType lType = getInputFromString(lKeys.at(k));

				if(lType != NONE)
					lComboKey.push_back(lType);
			}
			//*********************//

			int lPrerequis = NONE;
			bool lProjectileSpawner = false;

			if(lData.count("prerequis") > 0)
			{
				lPrerequis = getInputFromString(lData["prerequis"]);
			}

			if(lData.count("projectile") > 0 && lData["projectile"] == "true")
				lProjectileSpawner = true;

			addAnimation(lAnim, mFolder+"/combos/"+lFolders.at(i)+"/"+lAnim, lSpeed, false);

			Combo* lCmb = new Combo(lComboKey, lAnim, this, preHit, active, recup, damage, lZone, lStunTime, lPrerequis, lProjectileSpawner);
			
			if(lData.count("sound") > 0)
				lCmb->setSoundExecute(mFolder+"/combos/"+lFolders.at(i)+"/"+lData["sound"]);

			if(lData.count("soundHit") > 0)
				lCmb->setSound(mFolder+"/combos/"+lFolders.at(i)+"/"+lData["soundHit"]);

			lCmb->setID(stringToFloat(lData["id"]));

			std::vector<std::string> lInteruptedZone = split(lData["idInter"].ToAnsiString().substr(1, lData["idInter"].ToAnsiString().size()-2), ',');
			for(int i = 0; i < lInteruptedZone.size(); i++)
			{
				lCmb->addIDInterupted(stringToFloat(lInteruptedZone.at(i)));
			}

			
			mCombos.push_back(lCmb);
		}
		catch(std::exception e)
		{
			toDebugLog(sf::String("ERROR LOADING COMBO FOR : ") + mFolder + "\n" + e.what() + "\n\n");
		}
	}
}

//------------------------------------------------

void Character::loadUltra()
{
	std::map<sf::String, sf::String> lData = parseDataFile(mFolder+"/ultra/ultra.conf");

	try
	{
		mUltraAnim = lData["animation"];
		float lSpeed = stringToFloat(lData["speed"]);
		mUltraNbHit = stringToFloat(lData["nbHit"]);
		mUltraDegatByHit = stringToFloat(lData["degat"]);

		mUltraTimeBetweenHit = lSpeed / ((float)mUltraNbHit);
		
		addAnimation(mUltraAnim, mFolder+"/ultra/"+mUltraAnim, lSpeed, false);

		std::vector<std::string> lKeys;
		
		if(mGrosCasuMode)
		{
			lKeys = split(lData["comboCasu"].ToAnsiString().substr(1, lData["comboCasu"].ToAnsiString().size()-2), ',');
		}
		else
		{
			lKeys = split(lData["combo"].ToAnsiString().substr(1, lData["combo"].ToAnsiString().size()-2), ',');
		}
		
		std::list<InputType> lComboKey;

		for(int k = 0; k < lKeys.size(); k++)
		{
			InputType lType = getInputFromString(lKeys.at(k));

			if(lType != NONE)
				lComboKey.push_back(lType);
		}

		mUltraCombo = new Combo(lComboKey, mUltraAnim, this, 0,0,0,0,0,0,0,false, true);

		if(lData.count("sound") > 0)
				mUltraCombo->setSoundExecute(mFolder+"/ultra/"+lData["sound"]);

		if(!mGrosCasuMode)
			AddCombo(mUltraCombo);
	}
	catch(std::exception e)
	{
		toDebugLog(sf::String("ERROR LOADING ULTRA FOR : ") + mFolder + "\n" + e.what() + "\n\n");
	}

}

//-------------------------------------------

void Character::reset()
{
	mGrounded = false;

	mCurrentCombo = NULL;
	mUltraButtonDisplayed = NULL;

	mHit = false;
	mBlocking = false;
	mCrouching = false;

	mCanControl = true;

	mPV = mPvMax;

	if(mPlayer->isLeftSide())
	{
		setPosition(sf::Vector2f(100, SMFFEConfig::instance().getViewHeight()-mSprite->GetSize().y));
	}
	else
	{
		setPosition(sf::Vector2f(SMFFEConfig::instance().getViewWidth() - 100, SMFFEConfig::instance().getViewHeight()-mSprite->GetSize().y));
	}

	mPlayer->getInput()->clearCommandBuffer();
}