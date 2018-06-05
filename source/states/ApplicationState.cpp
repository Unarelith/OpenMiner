/*
 * =====================================================================================
 *
 *       Filename:  ApplicationState.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  14/12/2014 13:50:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include "Application.hpp"
#include "ApplicationState.hpp"
#include "Config.hpp"
#include "Mouse.hpp"

void ApplicationState::handleEvents() {
	Mouse::reset();

	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) {
		switch(event.type) {
			case SDL_QUIT:
				Application::getInstance().window().close();
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					Application::getInstance().window().close();
				}
				break;
			case SDL_MOUSEMOTION:
				Mouse::update(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				break;
			default:
				break;
		}
	}
}

