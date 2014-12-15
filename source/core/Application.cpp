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
#include <SFML/Window/Event.hpp>

#include "Application.hpp"
#include "Config.hpp"
#include "Exception.hpp"
#include "OpenGL.hpp"

Application::Application() {
	if(glewInit() != GLEW_OK) {
		EXCEPTION("glew init failed");
	}
	
	m_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), APP_NAME, sf::Style::Close, sf::ContextSettings(24, 8, 2));
	m_defaultView.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	
	//ResourceHandler::getInstance().loadResources();
	m_applicationStateStack = &ApplicationStateStack::getInstance();
}

Application::~Application() {
}

void Application::handleEvents() {
	sf::Event event;
	while(m_window.pollEvent(event)) {
		if(event.type == sf::Event::Closed) {
			m_window.close();
		}
		
		if(event.type == sf::Event::KeyPressed
		&& event.key.code == sf::Keyboard::Escape) {
			m_window.close();
		}
	}
}

void Application::run() {
	while(m_window.isOpen()) {
		m_clock.measureLastFrameDuration();
		
		handleEvents();
		
		m_clock.updateGame([&]{
			m_applicationStateStack->top().update();
		});
		
		m_clock.drawGame([&]{
			m_window.clear();
			m_window.setView(m_defaultView);
			
			m_applicationStateStack->top().draw();
			
			m_window.display();
		});
	}
}

