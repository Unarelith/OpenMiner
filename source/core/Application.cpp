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
#include <cstdlib>
#include <ctime>

#include "Application.hpp"
#include "Config.hpp"
#include "Exception.hpp"
#include "GameState.hpp"
#include "Mouse.hpp"
#include "OpenGL.hpp"
#include "SDLHeaders.hpp"

void Application::init() {
	CoreApplication::init();

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setMouseCursorGrabbed(true);
	m_window.setMouseCursorVisible(false);
	m_window.setVerticalSyncEnabled(true);

	m_renderer.init(m_window);

	m_stateStack.push<GameState>();
}

