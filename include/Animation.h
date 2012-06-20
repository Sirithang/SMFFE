#ifndef SMFFE_ANIMATION_H
#define SMFFE_ANIMATION_H


#include <SFML/Graphics.hpp>
#include <vector>



//----------------------------------------------
// Class d'animation.
// Gère la list des textures d'une animation.
//----------------------------------------------
class AnimatedSprite;

class Animation 
{
private:
	sf::String	mName;

	std::vector<sf::Texture*> mListTexture;

	std::vector<sf::Image*> mMaskList;

	int mCurrentFrame;
	float mTempsFrame;
	float mTimeStart;
	float mSpeed;
	bool mStart;

	bool mLoop;

	AnimatedSprite* mSprite;

public:
	Animation(AnimatedSprite* pSprite, const sf::String& pName, const sf::String& pPath, float pSpeed, bool pLoop = true);
	~Animation();

	void update();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Play the current animation </summary>
	/// 
	/// <returns>	Renvoi 0 si l'animation s'est bien déroulée sinon -1 </returns>
	///-------------------------------------------------------------------------------------------------
	void play();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Stop the current animation </summary>
	/// 
	/// <returns>	Renvoi 0 si l'animation s'est bien terminée sinon -1 </returns>
	///-------------------------------------------------------------------------------------------------
	void stop();

	int getCurrentFrame() const { return mCurrentFrame;}

	bool finished() const { return mCurrentFrame == mListTexture.size()-1; }

	void setSpeed(float pSpeed) {  mSpeed = pSpeed; mTempsFrame = mSpeed / mListTexture.size();}

	void flip();
};

#endif