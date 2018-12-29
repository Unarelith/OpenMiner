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
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>

#include "Application.hpp"
#include "Config.hpp"
#include "GameState.hpp"
#include "TextureLoader.hpp"

void Application::init() {
	gk::CoreApplication::init();

	gk::GamePad::init(m_keyboardHandler);

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");

	Registry::setInstance(m_registry);
	m_scriptEngine.init();

	m_stateStack.push<GameState>();
}

