#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "SMFFEConfig.h"
#include "constants.h"

#include "ScreenManager.h"
#include "StartScreen.h"
#include "CombatScreen.h"
#include "SelectCharScreen.h"
#include "Character.h"
#include "SMFFETime.h"

#include "Player.h"
#include "JoypadInput.h"
#include "KeyboardInput.h"

#include <iostream>

int main()
{
    bool gIsRunning = true;

	sf::RenderWindow win;

	if(SMFFEConfig::instance().isFullscreen())
	{
		win.Create(sf::VideoMode(	sf::VideoMode::GetDesktopMode().Width,
											sf::VideoMode::GetDesktopMode().Height, 32), "SMFFE", sf::Style::Fullscreen);
	}
	else
	{
		win.Create(sf::VideoMode(	SMFFEConfig::instance().getWinWidth(),
											SMFFEConfig::instance().getWinHeight(), 32), "SMFFE", sf::Style::Titlebar);
	}

	win.SetFramerateLimit(60);
	ScreenManager lManager(&win);

	//lManager.pushScreen(new SelectCharScreen());
	//lManager.pushScreen(new CombatScreen());
	lManager.pushScreen(new StartScreen());
	
	lManager.loop();
	
	std::cout<<"Quitting game"<<std::endl;

    return 0;
}