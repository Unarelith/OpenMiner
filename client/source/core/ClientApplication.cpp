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
#include <gk/gl/GLCheck.hpp>
#include <gk/graphics/Font.hpp>

#include "ClientApplication.hpp"
#include "Config.hpp"
#include "TextureLoader.hpp"

#include "GameState.hpp"
#include "TitleScreenState.hpp"
#include "ServerLoadingState.hpp"

using namespace std::literals::string_literals;

ClientApplication::ClientApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
}

void ClientApplication::init() {
	m_argumentParser.addArgument("host", {"-h", "--host", true});
	m_argumentParser.addArgument("port", {"-p", "--port", true});

	gk::CoreApplication::init();

	if (m_argumentParser.getArgument("host").isFound)
		m_host = m_argumentParser.getArgument("host").parameter;
	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	gk::GamePad::init(m_keyboardHandler);

	createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
	m_window.setVerticalSyncEnabled(true);
	m_window.disableView();

	initOpenGL();

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<gk::Font>("font-default", "resources/fonts/default.ttf");

	Registry::setInstance(m_registry);

	m_stateStack.push<TitleScreenState>();
	// m_stateStack.push<GameState>(m_host, m_port);
	// m_stateStack.push<ServerLoadingState>(m_client);
}

void ClientApplication::initOpenGL() {
	// Enable transparency
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Enable depth and hide backside of faces
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_CULL_FACE));

	glCheck(glEnable(GL_POLYGON_OFFSET_FILL));
	glCheck(glPolygonOffset(1, 1));

	// Set best quality for mipmaps
	glCheck(glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST));

	// Set clear color to skyblue
	glCheck(glClearColor(0.196078, 0.6, 0.8, 1.0));
}

