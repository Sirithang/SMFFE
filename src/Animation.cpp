#include "Animation.h"

#include "SMFFETime.h"
#include <dirent.h>

#include "AnimatedSprite.h"

Animation::Animation(AnimatedSprite* pSprite, const sf::String& pName,  const sf::String& pPath, float pSpeed, bool pLoop)
{
	mName = pName;

	mSprite = pSprite;
	mSpeed = pSpeed;
	mStart = false;

	mLoop = pLoop;

	DIR *lDir;
	struct dirent *ent;
	sf::Texture *lTexture = new sf::Texture();
	sf::Image *lImage;
	std::string lPath = pPath;

	// *********** IMAGE LOADING ****/////////////////////////
	lDir = opendir (lPath.c_str());

	if (lDir != NULL) 
	{
		for(int i =0;i=mListTexture.size();++i)
		{
			delete mListTexture.at(i);
		}

	  /* print all the files and directories within directory */
	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_REG)
		  {
			  sf::String lStr(ent->d_name);
			  if(lStr.Find(".png") != sf::String::InvalidPos || lStr.Find(".jpg") != sf::String::InvalidPos)
			  {
				  lTexture = new sf::Texture();
				  lTexture->LoadFromFile(pPath+"/"+sf::String(ent->d_name));
				  mListTexture.push_back(lTexture);
			  }
		  }
	  }
	  closedir (lDir);

	  mTempsFrame = mSpeed / mListTexture.size();
	} 
	else 
	{
	  /* could not open directory */
	  perror ("could not open directory");
	  return;
	}

	// *********** MASK LOADING ****/////////////////////////
	
	std::string lMaskPath = (sf::String(lPath)+sf::String("_mask")).ToAnsiString();
	lDir = opendir (lMaskPath.c_str());

	if (lDir != NULL) 
	{
		for(int i =0;i=mMaskList.size();++i)
		{
			delete mMaskList.at(i);
		}

	  /* print all the files and directories within directory */
	  while ((ent = readdir (lDir)) != NULL) 
	  {
		  if(ent->d_type == DT_REG)
		  {
			  sf::String lStr(ent->d_name);
			  if(lStr.Find(".png") != sf::String::InvalidPos || lStr.Find(".jpg") != sf::String::InvalidPos)
			  {
				  sf::Image* lImg = new sf::Image();
				  lImg->LoadFromFile(lMaskPath+"/"+sf::String(ent->d_name));
				  mMaskList.push_back(lImg);
			  }
		  }
	  }
	  closedir (lDir);

	  mTempsFrame = mSpeed / mListTexture.size();
	} 
	else 
	{
	  /* could not open directory */
	  perror ("could not open directory for mask");
	  //return;
	}

	//------------------------------------------------------------
		
	mSprite->mSprite->SetTexture(*mListTexture.at(0));

	if(mMaskList.size() > 0)
		mSprite->mCollisionMask = mMaskList.at(0);
}

Animation::~Animation()
{
	for(int i=0;i < mListTexture.size();i++)
	{
		delete mListTexture.at(i);
	}

	for(int i = 0; i < mMaskList.size(); i++)
	{
		delete mMaskList.at(i);
	}
}

//--------------------------------------------


void Animation::update()
{
	bool mWasChanged = false;

	if(mStart)
	{

		mTimeStart += Time::deltaTime();
	

		while(mTimeStart > mTempsFrame)
		{
			mCurrentFrame++;
			mWasChanged = true;
			if(mCurrentFrame >= mListTexture.size())
			{
				if(mLoop)
					mCurrentFrame = 0;
				else
				{
					mCurrentFrame = mListTexture.size() - 1;
					mStart = false;
					return;
				}
			}

			mTimeStart -= mTempsFrame;
		}
	}


	mSprite->mSprite->SetTexture(*mListTexture.at(mCurrentFrame), true);

	if(mMaskList.size() > 0)
		mSprite->mCollisionMask = mMaskList.at(mCurrentFrame);

	if(mSprite->mWasFliped)
	{
		mSprite->mSprite->SetOrigin(mSprite->mSprite->GetSubRect().Width, 0);
		//mSprite->mCollisionMask->FlipHorizontally();
	}
}


//--------------------------------------------

void Animation::play()
{
	mTimeStart = 0;
	mCurrentFrame = 0;
	mStart = true;
}

//--------------------------------------------

void Animation::stop()
{
	mStart = false;
}

//----------------------------------------------

void Animation::flip()
{
	/*for(int i = 0; i < mMaskList.size(); i++)
	{
		mMaskList[i]->FlipHorizontally();
	}*/
}