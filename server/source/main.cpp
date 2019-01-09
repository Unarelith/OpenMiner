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
#include "ServerApplication.hpp"

int main(int argc, char **argv) {
	ServerApplication app(argc, argv);
	app.run();

	return 0;
}

