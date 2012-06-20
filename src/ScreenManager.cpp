#include "..\include\ScreenManager.h"
#include "SMFFETime.h"

#include "SMFFEConfig.h"
#include "AScreen.h"


using namespace std;

ScreenManager::ScreenManager(sf::RenderWindow* pWindow)
	:mWindow(pWindow)
{
	sf::View lView(sf::Vector2f(0, 0), sf::Vector2f(SMFFEConfig::instance().getViewWidth(), SMFFEConfig::instance().getViewHeight()));
	mWindow->SetView(lView);
}


ScreenManager::~ScreenManager(void)
{
}

//--------------------------------------------------

void ScreenManager::loop()
{
	while (mWindow->IsOpened())
    {
		Time::tick();

		//*********Event Polling**********
		sf::Event event;
		while (mWindow->PollEvent(event))
		{
			// Request for closing the window
			if (event.Type == sf::Event::Closed)
				mWindow->Close();
		}
		//******************************

		mWindow->Clear();

		update();
		draw();

		mWindow->Display();
    }

	std::cout<<"Exiting Loop"<<std::endl;

	list<AScreen*>::iterator lIt = mScreenList.begin();
	while(lIt != mScreenList.end())
	{
		delete (*lIt);
		lIt++;
	}
}

//--------------------------------------------------

void ScreenManager::update()
{
	std::list<ScreenDeleteDelayer>::iterator lDelIt = mToPop.begin();

	while(lDelIt != mToPop.end())
	{
		lDelIt->mLifeTime -= Time::deltaTime();

		if(!lDelIt->mPoped)
		{
			mScreenList.remove(lDelIt->toDelete);
			lDelIt->mPoped = true;
		}

		if(lDelIt->mLifeTime <= 0)
		{
			delete lDelIt->toDelete;
			lDelIt = mToPop.erase(lDelIt);
		}
		else
		{
			lDelIt++;
		}
	}

	list<AScreen*>::iterator lIt = mScreenList.begin();

	if(lIt == mScreenList.end())
		return;

	//update le premier ecran 
	(*lIt)->update();
	lIt++;

	while(lIt != mScreenList.end())
	{
		if(!(*lIt)->isFrozen())
			(*lIt)->update();

		lIt++;
	}

}

//--------------------------------------------------

void ScreenManager::draw()
{
	list<AScreen*>::iterator lIt = mScreenList.end();

	while(lIt != mScreenList.begin())
	{
		lIt--;
		mWindow->SetView((*lIt)->getView());
		(*lIt)->draw(mWindow);
	}
}

//--------------------------------------------------

void ScreenManager::pushScreen(AScreen* pScreen)
{
	if(!mScreenList.empty())
		mScreenList.front()->freeze(true);

	mWindow->SetView(pScreen->getView());
	pScreen->setManager(this);

	mScreenList.push_front(pScreen);
}

//--------------------------------------------------

AScreen* ScreenManager::popScreen()
{
	if(mScreenList.empty())
		return NULL;

	ScreenDeleteDelayer lDelayer;
	lDelayer.mLifeTime = 2.0f;
	lDelayer.toDelete = mScreenList.front();
	mToPop.push_back(lDelayer);

	return mScreenList.front();
}

//--------------------------------------------------

AScreen* ScreenManager::peekScreen()
{
	if(mScreenList.empty())
		return NULL;

	return mScreenList.front();
}

//--------------------------------------------------
 
AScreen* ScreenManager::getScreen(unsigned int pIndex)
{
	if(pIndex < 0 || pIndex >= mScreenList.size())
		return NULL;

	int i = 0;
	list<AScreen*>::iterator lIt = mScreenList.begin();

	while(lIt != mScreenList.end())
	{
		if(i == pIndex)
			return (*lIt);

		i++;
		lIt++;
	}

	return NULL;
}