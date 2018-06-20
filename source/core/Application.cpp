/*
 * =====================================================================================
 *
 *       Filename:  Application.cpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:09:21
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Application.hpp"
#include "Config.hpp"
#include "Mouse.hpp"

#include "GameState.hpp"

void Application::init() {
	CoreApplication::init();

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);

	Mouse::setCursorVisible(false);
	Mouse::setCursorGrabbed(true);

	m_renderer.init(m_window);

	m_stateStack.push<GameState>();
}

