/*
 * =====================================================================================
 *
 *       Filename:  Application.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/12/2014 05:09:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <ctime>

#include <SFML/Window/Event.hpp>

#include "Application.hpp"
#include "Exception.hpp"
#include "Mouse.hpp"
#include "OpenGL.hpp"

Application::Application() : m_stateStack(ApplicationStateStack::getInstance()) {
	srand(time(NULL));
	
	m_window.open("KubKraft", 640, 480);
	m_window.setMouseCursorGrabbed(true);
	m_window.setMouseCursorVisible(false);
	m_window.setVerticalSyncEnabled(true);
	
	initGL();
}

void Application::initGL() {
#ifdef __MINGW32__
	if(glewInit() != GLEW_OK) {
		throw EXCEPTION("glew init failed");
	}
#endif
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	
	glClearColor(0.196078, 0.6, 0.8, 1.0); // Skyblue
}

void Application::handleEvents() {
	Mouse::reset();
	
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) {
		switch(event.type) {
			case SDL_QUIT:
				m_window.close();
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					m_window.close();
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

void Application::run() {
	while(m_window.isOpen()) {
		m_clock.measureLastFrameDuration();
		
		handleEvents();
		
		m_clock.updateGame([&]{
			m_stateStack.top().update();
		});
		
		m_clock.drawGame([&]{
			m_window.clear();
			
			m_stateStack.top().draw();
			
			m_window.display();
		});
	}
}

