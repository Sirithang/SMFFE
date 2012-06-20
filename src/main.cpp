#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "SMFFEConfig.h"
#include "constants.h"

#include "ScreenManager.h"
#include "SplashScreen.h"
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
	win.ShowMouseCursor(false);

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
	lManager.pushScreen(new SplashScreen());
	
	lManager.loop();
	
	std::cout<<"Quitting game"<<std::endl;

    return 0;
}