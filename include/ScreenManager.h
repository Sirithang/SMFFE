#ifndef SMFFE_SCREENMANAGER_H
#define SMFFE_SCREENMANAGER_H

#include <SFML/Graphics.hpp>

#include <list>

class AScreen;

struct ScreenDeleteDelayer
{
	AScreen*	toDelete;
	float		mLifeTime;
	bool		mPoped;

	ScreenDeleteDelayer()
	{
		mPoped = false;
	}
};

//----------------------------------------------
// Class gérant les screens.
// Tout les screens sont déssinés.
// Seul celui au dessus de la pile est updaté
// sauf si un screen précédent est explicitement 
// désigné comme non "freeze"
//----------------------------------------------
class ScreenManager
{
private:
	std::list<AScreen*> mScreenList;
	sf::RenderWindow*	mWindow;

	std::list<ScreenDeleteDelayer> mToPop;

public:
	ScreenManager(sf::RenderWindow* pWindow);
	~ScreenManager(void);

	/// <summary>	Loops jusqu'a ce que la fenetre soit fermé. </summary>
	void loop();

	void update();
	void draw();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Pushes a screen in the pile. </summary>
	/// 
	/// <param name="pScreen"> L'ecran que l'appellant alloue. </param>
	///-------------------------------------------------------------------------------------------------
	
	void	pushScreen(AScreen* pScreen);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Pops the screen. </summary>
	///
	/// <returns>	null si echoue, sinon le screen qu'il vient de poper. /!\ l'apellant de cette fonction doit DESALLOUER le screen lui même! </returns>
	///-------------------------------------------------------------------------------------------------
	
	AScreen* popScreen();

	///-------------------------------------------------------------------------------------------------
	/// <summary>   peek screen. </summary>
	///
	/// <returns>	null si vide, sinon le screen au dessus de la pile. </returns>
	///------------------------------------------------------------------------------------------------
	
	AScreen* peekScreen();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets a screen. </summary>
	///
	/// <param name="pIndex">	Zero-based index of the p. </param>
	///
	/// <returns>	null si l'index n'est pas valide ou si la liste est vide. Sinon l'ecran pIndex dans la pile (0 etant le dessus de la pile). </returns>
	///-------------------------------------------------------------------------------------------------

	AScreen* getScreen(unsigned int pIndex);


	sf::RenderWindow* getWin() { return mWindow; }
};

#endif

