/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/gl/GLCheck.hpp>
#include <gk/graphics/Font.hpp>

#include "ClientApplication.hpp"
#include "Config.hpp"
#include "TextureAtlas.hpp"
#include "TextureLoader.hpp"

#include "GameState.hpp"
#include "ServerLoadingState.hpp"
#include "TitleScreenState.hpp"

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

	m_keyboardHandler.loadKeysFromFile("resources/config/keys.xml");
	gk::GamePad::init(m_keyboardHandler);

	createWindow(Config::screenWidth, Config::screenHeight, APP_NAME);
	m_window.setVerticalSyncEnabled(true);
	m_window.disableView();

	initOpenGL();

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<gk::Font>("font-default", "resources/fonts/default.ttf");
	m_resourceHandler.add<TextureAtlas>("atlas-blocks");

	Registry::setInstance(m_registry);

	// m_stateStack.push<TitleScreenState>();

	m_stateStack.push<GameState>(m_host, m_port);

	// auto &game = m_stateStack.push<GameState>(m_host, m_port);
	// m_stateStack.push<ServerLoadingState>(game);
}

void ClientApplication::handleEvents() {
	gk::CoreApplication::handleEvents();

	if ((Config::isFullscreenModeEnabled && m_window.getWindowMode() != gk::Window::Mode::Fullscreen)
	|| (!Config::isFullscreenModeEnabled && m_window.getWindowMode() != gk::Window::Mode::Windowed)) {
		m_window.setWindowMode(Config::isFullscreenModeEnabled ? gk::Window::Mode::Fullscreen : gk::Window::Mode::Windowed);
	}

	if (Config::screenWidth != m_window.getSize().x || Config::screenHeight != m_window.getSize().y) {
		m_window.resize(Config::screenWidth, Config::screenHeight);
	}
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
}

