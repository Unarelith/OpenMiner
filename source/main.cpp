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
#include "Debug.hpp"
#include "Exception.hpp"
#include "SDLManager.hpp"

int main(int, char *[]) {
	try {
		SDLManager::init();

		Application &app = Application::getInstance();

		app.run();
	}
	catch(const Exception &err) {
		std::cerr << Debug::textColor(Debug::TextColor::Red, true) << "Fatal error " << Debug::textColor() << err.what() << std::endl;
		return 1;
	}
	catch(const std::exception &e) {
		std::cerr << Debug::textColor(Debug::TextColor::Red, true) << "Exception caught: " << Debug::textColor(0, true) << e.what() << Debug::textColor() << std::endl;
		return 1;
	}
	catch(...) {
		std::cerr << "Fatal error: Unknown error." << std::endl;
		return 1;
	}

	SDLManager::free();

	return 0;
}

