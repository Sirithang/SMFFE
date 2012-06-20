#include "Sprite.h"

#include "SMFFETime.h"

#include "constants.h"
#include <iostream>


Sprite::Sprite(const std::string& pImagePath, const sf::Vector2f pPosition)
{
	mSprite = new sf::Sprite();
	mTexture = new sf::Texture();

	mCollisionMask = NULL;

	if(pImagePath != "")
	{
		mTexture->LoadFromFile(pImagePath);
		mSprite->SetTexture(*mTexture);
	}

	mWasFliped = false;


	mSprite->SetPosition(pPosition);
	mToDelete = false;
}

Sprite::~Sprite()
{
	delete mTexture;
	delete mSprite;
}

void Sprite::draw(sf::RenderTarget* pTarget) const
{
	pTarget->Draw(*mSprite);
}

void Sprite::update()
{

}

//--------------------------------------------

bool Sprite::testMaskAgainst(Sprite *pSprite, bool pOverwriteCollisionOther)
{

	sf::Vector2f lPosition = mSprite->GetPosition();
	if(mWasFliped)
	{//if flipped, we put the origin once again on the top left
		lPosition.x -= mSprite->GetSize().x;
	}
	const sf::FloatRect lRect1(lPosition.x,lPosition.y, mSprite->GetSize().x, mSprite->GetSize().y);

	lPosition = pSprite->mSprite->GetPosition();
	if(pSprite->mWasFliped)
	{//if flipped, we put the origin once again on the top left
		lPosition.x -= pSprite->mSprite->GetSize().x;
	}
	const sf::FloatRect lRect2(lPosition.x,lPosition.y, pSprite->mSprite->GetSize().x, pSprite->mSprite->GetSize().y);

	// Espace de collision potentiel
	sf::FloatRect lZoneCollision;

	if(mLastCollisionResult.mMask != NULL)
	{
		delete mLastCollisionResult.mMask; 
		mLastCollisionResult.mMask = NULL;
		mLastCollisionResult.mCollided = false;
		mLastCollisionResult.mHit = false;
		mLastCollisionResult.mPenetrationFactor = sf::Vector2f(0,0);
		mLastCollisionResult.mBehind = false;
	}

	if(pOverwriteCollisionOther)
	{
		if(pSprite->mLastCollisionResult.mMask != NULL)
		{
			delete pSprite->mLastCollisionResult.mMask; 
			pSprite->mLastCollisionResult.mMask = NULL;
			pSprite->mLastCollisionResult.mCollided = false;
			pSprite->mLastCollisionResult.mHit = false;
			pSprite->mLastCollisionResult.mPenetrationFactor = sf::Vector2f(0,0);
		}
	}

	if(lRect1.Intersects(lRect2, lZoneCollision))
	{
		int lWidth = (int)lZoneCollision.Width;
		int lHeight = (int)lZoneCollision.Height;

		if(lWidth < 1 || lHeight < 1)
			return false;

		int lTop1 = (int)(lZoneCollision.Top - lRect1.Top);
		int lTop2 = (int)(lZoneCollision.Top - lRect2.Top);

		int lLeft1 = (int)(lZoneCollision.Left - lRect1.Left);
		int lLeft2 = (int)(lZoneCollision.Left - lRect2.Left);

		
		mLastCollisionResult.mMask = new sf::Image();
		mLastCollisionResult.mMask->Create(lWidth, lHeight);

		
		pSprite->mLastCollisionResult.mMask = new sf::Image();
		pSprite->mLastCollisionResult.mMask->Create(lWidth, lHeight);

		sf::Color color;
		sf::Color color2;
		
		color.a = 255;
		color.g = 0;

		color2.a = 255;
		color2.g = 0;

		sf::Vector2f mLastIndexColor(-1, -1);
		sf::Vector2f mLastIndexColor2(-1, -1);

		sf::Vector2i mMinimumHitBox(-1, -1);
		sf::Vector2i mMinimumHitBox2(-1, -1);

		sf::Vector2i mMaximumHitBox(-1, -1);
		sf::Vector2i mMaximumHitBox2(-1, -1);

		float invertScale1 = 1.0f / mSprite->GetScale().x;
		float invertScale2 = 1.0f / pSprite->mSprite->GetScale().x;

		if(lHeight > mCollisionMask->GetHeight() || lHeight > pSprite->mCollisionMask->GetHeight() ||
			lWidth > mCollisionMask->GetWidth() || lWidth > pSprite->mCollisionMask->GetWidth())
			return false;

		for(int lIndexY = 0; lIndexY < lHeight; lIndexY++)
		{
			for(int lIndexX = 0; lIndexX < lWidth; lIndexX ++)
			{
				unsigned char r1;
				
				if(mWasFliped)
				{
					r1 = mCollisionMask->GetPixel((mCollisionMask->GetWidth() - 1) - (lIndexX+lLeft1) * invertScale1, (lIndexY+lTop1) * invertScale1).r;
				}
				else
				{
					r1 = mCollisionMask->GetPixel((lIndexX+lLeft1) * invertScale1, (lIndexY+lTop1) * invertScale1).r;
				}

				unsigned char r2;
				
				if(pSprite->mWasFliped)
				{
					r2 = pSprite->mCollisionMask->GetPixel((pSprite->mCollisionMask->GetWidth() - 1) - (lIndexX+lLeft2) * invertScale2, (lIndexY+lTop2) * invertScale2).r;
				}
				else
				{
					r2 = pSprite->mCollisionMask->GetPixel((lIndexX+lLeft2) * invertScale2, (lIndexY+lTop2) * invertScale2).r;
				}

				unsigned char b1;
				
				if(mWasFliped)
				{
					b1 = mCollisionMask->GetPixel((mCollisionMask->GetWidth() - 1) - (lIndexX+lLeft1) * invertScale1, (lIndexY+lTop1) * invertScale1).b;
				}
				else
				{
					b1 = mCollisionMask->GetPixel((lIndexX+lLeft1) * invertScale1, (lIndexY+lTop1) * invertScale1).b;
				}

				unsigned char b2;
				
				if(pSprite->mWasFliped)
				{
					b2 = pSprite->mCollisionMask->GetPixel((pSprite->mCollisionMask->GetWidth() - 1) -  (lIndexX+lLeft2) * invertScale2, (lIndexY+lTop2) * invertScale2).b;
				}
				else
				{
					b2 = pSprite->mCollisionMask->GetPixel((lIndexX+lLeft2) * invertScale2, (lIndexY+lTop2) * invertScale2).b;
				}

				color.r =  r1 & r2;
				color.b =  r1 & b2;

				mLastCollisionResult.mMask->SetPixel(lIndexX,lIndexY,color);

				if(color.r != 0)
				{
					if(mLastIndexColor.x == -1)
					{
						if(lIndexX == 0)
							mLastCollisionResult.mBehind = true;

						mLastIndexColor.x = lIndexX - 1;
					}

					if(mLastIndexColor.y == -1)
					{
						mLastIndexColor.y = lIndexY - 1;
					}

					if(lIndexX - mLastIndexColor.x > mLastCollisionResult.mPenetrationFactor.x)
						mLastCollisionResult.mPenetrationFactor.x = lIndexX - mLastIndexColor.x;

					if(lIndexY - mLastIndexColor.y > mLastCollisionResult.mPenetrationFactor.y)
						mLastCollisionResult.mPenetrationFactor.y = lIndexY - mLastIndexColor.y;

					mLastCollisionResult.mCollided = true;
				}

				if(color.b != 0)
				{
					mLastCollisionResult.mHit = true;
					mLastCollisionResult.mPosition = sf::Vector2f(lZoneCollision.Left + lIndexX,lZoneCollision.Top + lIndexY);

					if(mMinimumHitBox.x == -1)
					{
						mMaximumHitBox.x = lIndexX;
						mMinimumHitBox.x = lIndexX;
					}
					if(mMinimumHitBox.y == -1)
					{
						mMaximumHitBox.y = lIndexY;
						mMinimumHitBox.y = lIndexY;
					}

					if(lIndexX < mMinimumHitBox.x) mMinimumHitBox.x= lIndexX;
					else if(lIndexX > mMaximumHitBox.x) mMaximumHitBox.x = lIndexX;

					if(lIndexY < mMinimumHitBox.y) mMinimumHitBox.y = lIndexY;
					else if(lIndexY > mMaximumHitBox.y) mMaximumHitBox.y = lIndexY;
				}
				
				//********pSprite*****************

				if(pOverwriteCollisionOther)
				{
					color2.r = color.r;
					color2.b = b1 & r2;

					pSprite->mLastCollisionResult.mMask->SetPixel(lIndexX,lIndexY,color2);

					if(color2.r != 0)
					{
						if(mLastIndexColor2.x == -1)
						{
							if(lIndexX == 0)
								pSprite->mLastCollisionResult.mBehind = true;
							mLastIndexColor2.x = lIndexX - 1;
						}

						if(mLastIndexColor2.y == -1)
						{
							mLastIndexColor2.y = lIndexY - 1;
						}

						if(lIndexX - mLastIndexColor2.x > pSprite->mLastCollisionResult.mPenetrationFactor.x)
							pSprite->mLastCollisionResult.mPenetrationFactor.x = lIndexX - mLastIndexColor2.x;

						if(lIndexY - mLastIndexColor2.y > pSprite->mLastCollisionResult.mPenetrationFactor.y)
							mLastCollisionResult.mPenetrationFactor.y = lIndexY - mLastIndexColor2.y;

						pSprite->mLastCollisionResult.mCollided = true;
					}

					if(color2.b != 0)
					{
						pSprite->mLastCollisionResult.mHit = true;

						if(mMinimumHitBox2.x == -1)
						{
							mMaximumHitBox2.x = lIndexX;
							mMinimumHitBox2.x = lIndexX;
						}
						if(mMinimumHitBox2.y == -1)
						{
							mMaximumHitBox2.y = lIndexY;
							mMinimumHitBox2.y = lIndexY;
						}

						if(lIndexX < mMinimumHitBox2.x) mMinimumHitBox2.x= lIndexX;
						else if(lIndexX > mMaximumHitBox2.x) mMaximumHitBox2.x = lIndexX;

						if(lIndexY < mMinimumHitBox2.y) mMinimumHitBox2.y = lIndexY;
						else if(lIndexY > mMaximumHitBox2.y) mMaximumHitBox2.y = lIndexY;
					}

					pSprite->mLastCollisionResult.mPosition = sf::Vector2f(lZoneCollision.Left,lZoneCollision.Top);
				}
			}

			if(mLastCollisionResult.mHit)
			{
				sf::Vector2i rectTop(lLeft1, lTop1);
				mLastCollisionResult.mHitRect = sf::IntRect(rectTop + mMinimumHitBox, mMaximumHitBox - mMinimumHitBox + sf::Vector2i(1,1));
			}

			if(pOverwriteCollisionOther)
			{
				if(pSprite->mLastCollisionResult.mHit)
				{
					sf::Vector2i zoneTop(lZoneCollision.Left, lZoneCollision.Top);
					sf::Vector2i rectTop(lLeft2, lTop2);
					pSprite->mLastCollisionResult.mHitRect = sf::IntRect(rectTop + mMinimumHitBox2, mMaximumHitBox2 - mMinimumHitBox2 + sf::Vector2i(1,1));
				}
			}
		}
		return true;
	}

	return false;
}

//--------------------------------------------

CollisionResult Sprite::getLastCollision()
{
	return mLastCollisionResult;
}

//-----------------------------------------------


void Sprite::flipX(bool pFlip)
{
	if(pFlip == mWasFliped)
		return;

	mSprite->FlipX(pFlip);

	if(pFlip)
	{
		mSprite->SetOrigin(mSprite->GetSubRect().Width, 0); 
		setPosition(getPosition() + sf::Vector2f(mSprite->GetSize().x, 0));
	}
	else
	{
		mSprite->SetOrigin(0, 0); 
		setPosition(getPosition() - sf::Vector2f(mSprite->GetSize().x, 0));
	}
	mWasFliped = pFlip;
}