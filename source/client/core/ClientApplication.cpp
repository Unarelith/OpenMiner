/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "filesystem.hpp"

#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/gl/GLCheck.hpp>

#include "BlockGeometry.hpp"
#include "ClientApplication.hpp"
#include "Config.hpp"
#include "EngineConfig.hpp"
#include "Font.hpp"
#include "TextureAtlas.hpp"
#include "TextureLoader.hpp"
#include "Vertex.hpp"

#include "TitleScreenState.hpp"

namespace fs = ghc::filesystem;

ClientApplication::ClientApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	BlockGeometry::initOrientation();
}

void ClientApplication::init() {
	m_argumentParser.addArgument("host", {"-h", "--host", "Select the host to connect to.", "host"});
	m_argumentParser.addArgument("port", {"-p", "--port", "Select the port to use.", "port"});
	m_argumentParser.addArgument("singleplayer", {"-s", "--singleplayer", "Start in singleplayer mode."});
	m_argumentParser.addArgument("multiplayer", {"-m", "--multiplayer", "Start in multiplayer mode."});
	m_argumentParser.addArgument("working-dir", {"-w", "--working-dir", "Change the working directory to <dir>.", "dir"});
	m_argumentParser.addArgument("texture-pack", {"-t", "--texture-pack", "Use texture pack <name>.", "name"});
	m_argumentParser.addArgument("username", {"-u", "--username", "Use <username> when using -m or -s", "username"});

	m_loggerHandler.setName("client");

	gk::CoreApplication::init();

	m_window.addVertexAttribute(VertexAttribute::Coord3d, "coord3d", 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	m_window.addVertexAttribute(VertexAttribute::TexCoord, "texCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	m_window.addVertexAttribute(VertexAttribute::Color, "color", 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
	m_window.addVertexAttribute(VertexAttribute::Normal, "normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
	m_window.addVertexAttribute(VertexAttribute::LightValue, "lightValue", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, lightValue)));
	m_window.addVertexAttribute(VertexAttribute::AmbientOcclusion, "ambientOcclusion", 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, ambientOcclusion)));

	if (m_argumentParser.getArgument("help").isFound)
		return;

	if (m_argumentParser.getArgument("working-dir").isFound)
		fs::current_path(m_argumentParser.getArgument("working-dir").parameter);

	if (m_argumentParser.getArgument("host").isFound)
		m_host = m_argumentParser.getArgument("host").parameter;
	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	Config::loadConfigFromFile("config/client.lua");

	if (m_argumentParser.getArgument("username").isFound)
		Config::defaultUsername = m_argumentParser.getArgument("username").parameter;

	m_keyboardHandler.loadKeysFromFile("config/keys.lua");
	gk::GamePad::init(m_keyboardHandler);

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	createWindow(sf::VideoMode{Config::screenWidth, Config::screenHeight}, APP_NAME, sf::Style::Titlebar | sf::Style::Close, settings);
	m_window.setVerticalSyncEnabled(Config::isVerticalSyncEnabled);
	m_window.setOpenGLFlagsSetupFunc(&ClientApplication::initOpenGL);
	m_window.disableView();

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<Font>("font-ascii", "texture-font", "resources/textures/font.properties");
	m_resourceHandler.add<TextureAtlas>("atlas-blocks");

	auto &titleScreen = m_stateStack.push<TitleScreenState>(m_port);
	if (m_argumentParser.getArgument("texture-pack").isFound)
		titleScreen.setTexturePack(m_argumentParser.getArgument("texture-pack").parameter);
	if (m_argumentParser.getArgument("singleplayer").isFound)
		titleScreen.startSingleplayer(false);
	else if (m_argumentParser.getArgument("multiplayer").isFound)
		titleScreen.startMultiplayer(m_host);
}

void ClientApplication::handleEvents() {
	gk::CoreApplication::handleEvents();

	if ((Config::isFullscreenModeEnabled && !m_window.isFullscreenModeEnabled())
	|| (!Config::isFullscreenModeEnabled && m_window.isFullscreenModeEnabled())) {
		m_window.setFullscreenMode(Config::isFullscreenModeEnabled);
	}

	if (Config::screenWidth != m_window.getSize().x || Config::screenHeight != m_window.getSize().y) {
		m_window.setSize({Config::screenWidth, Config::screenHeight});
	}

	if (Config::isVerticalSyncEnabled != m_window.isVerticalSyncEnabled())
		m_window.setVerticalSyncEnabled(Config::isVerticalSyncEnabled);
}

void ClientApplication::onEvent(const sf::Event &event) {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11)
		Config::isFullscreenModeEnabled ^= 1;
}

void ClientApplication::onExit() {
	Config::saveConfigToFile("config/client.lua");

	m_keyboardHandler.saveKeysToFile("config/keys.lua");
}

void ClientApplication::initOpenGL() {
	// Enable textures
	glCheck(glEnable(GL_TEXTURE_2D));

	// Enable transparency
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Enable depth and hide backside of faces
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_CULL_FACE));

	// Set best quality for mipmaps
	glCheck(glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST));

	glCheck(glEnable(GL_POLYGON_OFFSET_FILL));
	glCheck(glPolygonOffset(1, 1));
}

