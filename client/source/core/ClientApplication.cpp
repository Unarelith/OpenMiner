/*
 * =====================================================================================
 *
 *       Filename:  ClientApplication.cpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:33:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/graphics/Font.hpp>

#include "ClientApplication.hpp"
#include "Config.hpp"
#include "GameState.hpp"
#include "TextureLoader.hpp"

void ClientApplication::init() {
	gk::CoreApplication::init();

	gk::GamePad::init(m_keyboardHandler);

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);
	m_window.disableView();

	initOpenGL();

	gk::Mouse::setCursorVisible(false);
	gk::Mouse::setCursorGrabbed(true);

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<gk::Font>("font-default", "resources/fonts/default.ttf");

	Registry::setInstance(m_registry);

	// m_scriptEngine.init();

	m_stateStack.push<GameState>();
}

void ClientApplication::initOpenGL() {
	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth and hide backside of faces
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	// Set best quality for mipmaps
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	// Set clear color to skyblue
	glClearColor(0.196078, 0.6, 0.8, 1.0);
}

