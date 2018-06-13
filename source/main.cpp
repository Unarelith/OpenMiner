/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  13/12/2014 20:49:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include <iostream>

#include "Application.hpp"
#include "Exception.hpp"
#include "SDLLoader.hpp"

int main(int, char *[]) {
	SDLLoader sdlLoader;

	try {
		sdlLoader.load();

		Application app;

		app.run();
	}
	catch(const Exception &e) {
		std::cerr << "Fatal error " << e.what() << std::endl;
		return 1;
	}
	// catch(const std::exception &e) {
	// 	std::cerr << "Exception caught: " << e.what() << std::endl;
	// 	return 1;
	// }
	// catch(...) {
	// 	std::cerr << "Fatal error: Unknown error." << std::endl;
	// 	return 1;
	// }

	return 0;
}

