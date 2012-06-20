#ifndef SMFFE_ANIMATEDSPRITE_H
#define SMFFE_ANIMATEDSPRITE_H


#include <SFML/Graphics.hpp>
#include <list>
#include <map>

#include "Sprite.h"
#include "Animation.h"

//----------------------------------------------
// Class du sprite animé.
//----------------------------------------------

class AnimatedSprite : public Sprite 
{
protected:
	std::map<sf::String,Animation*> mAnimations;
	std::list<sf::String> mCurrentAnimation;

public:
	AnimatedSprite(const sf::Vector2f pPosition);
	~AnimatedSprite(void);

	virtual void update();

	void addAnimation(const std::string& pName, const sf::String &pPath, float pSpeed, bool pLoop = true);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Play the current animation </summary>
	///-------------------------------------------------------------------------------------------------
	void play(std::string pName, bool pPop = true, bool pForceRestart = false);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Stop the current animation </summary>
	///-------------------------------------------------------------------------------------------------
	void stop();

	void stopAll();

	void flipX(bool pFlip);

	//Pop the animation on top of pile. If pRestart == true, restart the animation newt in line.
	void popAnimation(bool pRestart);

	Animation* getCurrentAnimation() { if(mCurrentAnimation.back() == "") return NULL; return mAnimations[mCurrentAnimation.back()]; }

	friend class Animation;
};


#endif