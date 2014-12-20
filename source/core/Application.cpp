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
#include "Config.hpp"
#include "Exception.hpp"
#include "OpenGL.hpp"

Application::Application() {
	srand(time(NULL));
	
	m_window.open(APP_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_window.setMouseCursorGrabbed(true);
	m_window.setMouseCursorVisible(false);
	m_window.setVerticalSyncEnabled(true);
	
	initGL();
	
	//ResourceHandler::getInstance().loadResources();
	m_applicationStateStack = &ApplicationStateStack::getInstance();
}

Application::~Application() {
}

void Application::initGL() {
#ifdef __MINGW32__
	if(glewInit() != GLEW_OK) {
		throw EXCEPTION("glew init failed");
	}
#endif
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	
	glClearColor(0.196078, 0.6, 0.8, 1.0); // Skyblue
}

void Application::run() {
	while(m_window.isOpen()) {
		m_clock.measureLastFrameDuration();
		
		m_applicationStateStack->top().handleEvents();
		
		m_clock.updateGame([&]{
			m_applicationStateStack->top().update();
		});
		
		m_clock.drawGame([&]{
			m_window.clear();
			
			m_applicationStateStack->top().draw();
			
			m_window.display();
		});
	}
}

