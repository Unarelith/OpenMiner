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

ClientApplication::ClientApplication(int argc, char **argv) : CoreApplication(argc, argv) {
	BlockGeometry::initOrientation();
}

bool ClientApplication::init() {
	m_argumentParser.addArgument("host",         {"-h", "--host",         "Select the host to connect to.", "host"});
	m_argumentParser.addArgument("port",         {"-p", "--port",         "Select the port to use.", "port"});
	m_argumentParser.addArgument("singleplayer", {"-s", "--singleplayer", "Start in singleplayer mode."});
	m_argumentParser.addArgument("multiplayer",  {"-m", "--multiplayer",  "Start in multiplayer mode."});
	m_argumentParser.addArgument("working-dir",  {"-w", "--working-dir",  "Change the working directory to <dir>.", "dir"});
	m_argumentParser.addArgument("texture-pack", {"-t", "--texture-pack", "Use texture pack <name>.", "name"});
	m_argumentParser.addArgument("username",     {"-u", "--username",     "Use <username> when using -m or -s", "username"});
	m_argumentParser.addArgument("log-level",    {"-l", "--log-level",    "Choose the log level (debug, info, warning, error)", "level"});

	m_loggerHandler.setName("client");

	fs::create_directory("logs");
	m_loggerHandler.openLogFile("logs/openminer.log");

	CoreApplication::init();

	if (m_argumentParser.getArgument("help").isFound)
		return true;

	if (m_argumentParser.getArgument("log-level").isFound) {
		std::unordered_map<std::string, gk::LogLevel> levels = {
			{"debug",   gk::LogLevel::Debug},
			{"info",    gk::LogLevel::Info},
			{"warning", gk::LogLevel::Warning},
			{"error",   gk::LogLevel::Error},
		};

		auto it = levels.find(m_argumentParser.getArgument("log-level").parameter);
		if (it != levels.end())
			m_loggerHandler.setMaxLevel(it->second);
		else
			gkWarning() << ("Failed to set log level to '" + m_argumentParser.getArgument("log-level").parameter + "': Invalid value").c_str();
	}

	if (m_argumentParser.getArgument("working-dir").isFound)
		fs::current_path(m_argumentParser.getArgument("working-dir").parameter);

	if (m_argumentParser.getArgument("host").isFound)
		m_host = m_argumentParser.getArgument("host").parameter;
	if (m_argumentParser.getArgument("port").isFound)
		m_port = (u16)std::stoi(m_argumentParser.getArgument("port").parameter);

	Config::loadConfigFromFile("config/client.lua");

	if (m_argumentParser.getArgument("username").isFound)
		Config::defaultUsername = m_argumentParser.getArgument("username").parameter;

	if (m_argumentParser.getArgument("texture-pack").isFound)
		Config::texturePack = m_argumentParser.getArgument("texture-pack").parameter;

	m_keyboardHandler.loadKeysFromFile("config/keys.lua");
	gk::GamePad::init(m_keyboardHandler);

	createWindow(Config::screenWidth, Config::screenHeight, APP_NAME);
	m_window.setVerticalSyncEnabled(Config::isVerticalSyncEnabled);
	m_window.disableView();

	initOpenGL();

	m_resourceHandler.loadConfigFile<TextureLoader>("resources/config/textures.xml");
	m_resourceHandler.add<Font>("font-ascii", "texture-font", "resources/textures/font.properties");
	m_resourceHandler.add<TextureAtlas>("atlas-blocks");

#ifdef OM_PROFILER_ENABLED
	ClientProfiler::setInstance(&m_profiler);
#endif

	auto &titleScreen = m_stateStack.push<TitleScreenState>(m_port);
	if (m_argumentParser.getArgument("singleplayer").isFound)
		titleScreen.startSingleplayer(false);
	else if (m_argumentParser.getArgument("multiplayer").isFound)
		titleScreen.startMultiplayer(m_host);

	return true;
}

void ClientApplication::handleEvents() {
	OM_PROFILE_START("OS events");

	CoreApplication::handleEvents();

	if ((Config::isFullscreenModeEnabled && m_window.getWindowMode() != gk::Window::Mode::Fullscreen)
	|| (!Config::isFullscreenModeEnabled && m_window.getWindowMode() != gk::Window::Mode::Windowed)) {
		m_window.setWindowMode(Config::isFullscreenModeEnabled ? gk::Window::Mode::Fullscreen : gk::Window::Mode::Windowed);
	}

	if (Config::screenWidth != m_window.getSize().x || Config::screenHeight != m_window.getSize().y) {
		m_window.resize(Config::screenWidth, Config::screenHeight);
	}

	if (Config::isVerticalSyncEnabled != m_window.isVerticalSyncEnabled())
		m_window.setVerticalSyncEnabled(Config::isVerticalSyncEnabled);

	OM_PROFILE_END("OS events");
}

void ClientApplication::onEvent(const SDL_Event &event) {
	CoreApplication::onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11)
		Config::isFullscreenModeEnabled ^= 1;
}

void ClientApplication::onExit() {
	Config::saveConfigToFile("config/client.lua");

	m_keyboardHandler.saveKeysToFile("config/keys.lua");
}

void ClientApplication::mainLoop() {
	m_clock.startFpsTimer();

	// FIXME: The window should probably be closed after the main loop ends
	while(m_window.isOpen() && !m_stateStack.empty() && !hasBeenInterrupted) {
		OM_PROFILE_BEGIN_TICK();

		handleEvents();

		m_eventHandler.processEvents();

		m_clock.updateGame([&] {
			OM_PROFILE_START("Update");

			if (!m_stateStack.empty())
				m_stateStack.top().update();

			m_stateStack.clearDeletedStates();

			OM_PROFILE_END("Update");
		});

		m_clock.drawGame([&] {
			m_window.clear();

			OM_PROFILE_START("Draw");

			if(!m_stateStack.empty())
				m_window.draw(m_stateStack.top(), m_renderStates);

			OM_PROFILE_END("Draw");

			m_window.display();
		});

		OM_PROFILE_END_TICK();
	}
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

