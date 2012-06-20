#ifndef SMFFE_SPRITE_H
#define SMFFE_SPRITE_H

#include <SFML/Graphics.hpp>
#include "CollisionResult.h"

//----------------------------------------------
// Class gérant les sprite.
// Les collisions du sprite sont aussi gérées.
//----------------------------------------------
class Sprite
{
protected:
	sf::Texture *mTexture;
	sf::Sprite *mSprite;
	sf::Image *mImage;
	sf::Image *mCollisionMask;
	CollisionResult mLastCollisionResult;
	std::string mName;

	bool mWasFliped;
	bool mToDelete;

public:
	Sprite(const std::string& pName, const sf::Vector2f pPosition);
	virtual ~Sprite();
	virtual void update();
	virtual void draw(sf::RenderTarget* pTarget) const;

	virtual void flipX(bool pFlip);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Test the Mask Collision </summary>
	/// 
	/// <param name="pMask"> le Sprite de collision</param>
	/// <param name="pOverwriteCollisionOther"> Si vrai (defaut) ecrit egalement le mask de collision
	///                                         de pSprite (permet de faire l'operation en une seul boucle)</param>
	///
	/// <returns>	Vrai si il y a collision et remplit le masque de collision du sprite </returns>
	///-------------------------------------------------------------------------------------------------
	
	bool testMaskAgainst(Sprite *pSprite, bool pOverwriteCollisionOther = true);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Retourne la dernière collision </summary>
	///
	/// <returns>	null si il n'y a pas eu de collision </returns>
	///-------------------------------------------------------------------------------------------------
	
	CollisionResult getLastCollision();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Retourne une reference vers le sprite </summary>
	///
	/// <returns>	Une reference  vers le sf::Sprite </returns>
	///-------------------------------------------------------------------------------------------------
	
	sf::Sprite& getSFSprite() { return *mSprite; }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Racourcis vers la position </summary>
	///
	/// <returns>	Un sf::Vector2f qui est la position du sprite </returns>
	///-------------------------------------------------------------------------------------------------
	
	const sf::Vector2f getPosition() { return mSprite->GetPosition(); }

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Test the Mask Collision </summary>
	/// 
	/// <param name="pPosition"> nouvelle position du sprite</param>
	///-------------------------------------------------------------------------------------------------

	void setPosition(const sf::Vector2f &pPosition) { mSprite->SetPosition(pPosition); }

	bool isToDelete() { return mToDelete; }
};

#endif