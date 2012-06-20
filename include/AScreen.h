#ifndef SMFFE_ASCREEN_H
#define SMFFE_ASCREEN_H

///----------------------------
/// Class abstraite designant un screen
/// (exemple : startscreen, menu, combat etc...)
/// a deriver pour chaque screen 
//-------------------------------------

#include <SFML/Graphics.hpp>

#include "ScreenManager.h"

class AScreen
{
protected:
	//Player* mController; TODO : implementer le player.
	//Si frozen, n'est pas updaté.
	bool mIsFrozen;

	sf::View mView;
	ScreenManager* mManager;

public:
	AScreen(void);
	virtual ~AScreen(void);

	virtual void update() = 0;
	virtual void draw(sf::RenderTarget* pTarget) = 0;

	void freeze(bool pIsFreeze) { mIsFrozen = pIsFreeze; }
	bool isFrozen() {return mIsFrozen;}

	const sf::View& getView() const { return mView; }

	void setManager(ScreenManager* pManager) { mManager = pManager; }
};

#endif