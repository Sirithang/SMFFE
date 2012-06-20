#ifndef SMFFE_COLLISIONRESULT_H
#define SMFFE_COLLISIONRESULT_H

#include <SFML/Graphics.hpp>

//----------------------------------------------
// Class gérant le masque de collision entre 
// deux sprites ainsi que sa position.
//----------------------------------------------

class CollisionResult
{
public:
	sf::Image* mMask;
	sf::Vector2f mPosition;
	sf::Vector2f mPenetrationFactor; // size of the "non black" collision
	bool mCollided;

	bool mBehind;

	bool mHit;
	sf::IntRect mHitRect;

	CollisionResult() { mMask = NULL; mCollided = false; mHit = false; mBehind = false;}
};

#endif