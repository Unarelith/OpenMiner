/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:
 *
 *        Created:  13/12/2014 20:49:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <iostream>

#include "Application.hpp"
#include "Exception.hpp"
#ifdef USE_SDL
	#include "SDLLoader.hpp"
#endif // USE_SDL

int main(int argc, char **argv) {
#ifdef USE_SDL
	SDLLoader sdlLoader;
	sdlLoader.load();
#endif // USE_SDL

	Application app(argc, argv);
	app.run();

	return 0;
}

