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
#include <gk/gui/Font.hpp>

#include "Application.hpp"
#include "Config.hpp"
#include "GameState.hpp"
#include "TextureLoader.hpp"

void Application::init() {
	gk::CoreApplication::init();

	gk::GamePad::init(m_keyboardHandler);

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);

	initOpenGL();

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<gk::Font>("font-default", "resources/fonts/default.ttf");

	Registry::setInstance(m_registry);
	m_scriptEngine.init();

	m_stateStack.push<GameState>();
}

void Application::initOpenGL() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	// glBlendFunc(GL_ONE, GL_ONE);
	// glBlendEquation(GL_FUNC_ADD);
	// glEnable(GL_ALPHA_TEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glClearColor(0.196078, 0.6, 0.8, 1.0); // Skyblue
}

