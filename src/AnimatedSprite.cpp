#include "AnimatedSprite.h"

#include "constants.h"

AnimatedSprite::AnimatedSprite(const sf::Vector2f pPosition)
	: Sprite("", pPosition)
{
	//mCurrentAnimation = "";
}

AnimatedSprite::~AnimatedSprite(void)
{
	std::map<sf::String,Animation*>::iterator lIt = mAnimations.begin();

	while(lIt != mAnimations.end())
	{
		delete lIt->second;
		lIt++;
	}

	mAnimations.clear();
}

//--------------------------------------------

void AnimatedSprite::flipX(bool pFlip)
{
	Sprite::flipX(pFlip);

	std::map<sf::String,Animation*>::iterator lIt = mAnimations.begin();
	while(lIt != mAnimations.end())
	{
		lIt->second->flip();
		lIt++;
	}
}

//--------------------------------------------

void AnimatedSprite::update()
{
	if(mCurrentAnimation.size() == 0 || mCurrentAnimation.back() == "")
		return;

	mAnimations[mCurrentAnimation.back()]->update();
}

//--------------------------------------------

void AnimatedSprite::play(std::string pName, bool pPop, bool pForceRestart)
{
	if(mCurrentAnimation.size() > 0 && (pName == mCurrentAnimation.back() && !pForceRestart))
		return;

	if(pPop)
	{
		popAnimation(false);

		if(mCurrentAnimation.size() > 0 && pName == mCurrentAnimation.back())
			return;
	}

	mCurrentAnimation.push_back(pName);
	mAnimations[pName]->play();
}

//-------------------------------------------

void AnimatedSprite::popAnimation(bool pRestart)
{
	if(mCurrentAnimation.size() == 0)
		return;

	mCurrentAnimation.pop_back();

	if(mCurrentAnimation.size() != 0 && pRestart)
		mAnimations[mCurrentAnimation.back()]->play();
}

//--------------------------------------------

void AnimatedSprite::stop()
{
	mAnimations[mCurrentAnimation.back()]->stop();
	mCurrentAnimation.pop_back();
}

//--------------------------------------------

void AnimatedSprite::stopAll()
{
	mAnimations[mCurrentAnimation.back()]->stop();
	mCurrentAnimation.clear();
}

//--------------------------------------------

void AnimatedSprite::addAnimation(const std::string& pName, const sf::String &pPath, float pSpeed, bool pLoop)
{
	if(mAnimations.count(pName) > 0)
	{
		mAnimations[pName]->setSpeed(pSpeed);
	}
	else
	{
		mAnimations[pName] = new Animation(this, pName, pPath, pSpeed, pLoop);
	}
}

