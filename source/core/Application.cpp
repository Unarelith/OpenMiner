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

Application::Application() {
	srand(time(NULL));

	m_window.open(APP_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_window.setMouseCursorGrabbed(true);
	m_window.setMouseCursorVisible(false);
	m_window.setVerticalSyncEnabled(true);

	m_renderer.init(m_window);

	//ResourceHandler::getInstance().loadResources();
	m_applicationStateStack = &ApplicationStateStack::getInstance();
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

