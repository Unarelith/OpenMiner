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
#include <filesystem.hpp>

#include "BlockGeometry.hpp"
#include "Events.hpp"
#include "GameTime.hpp"
#include "ServerApplication.hpp"
#include "ServerBlock.hpp"
#include "ServerConfig.hpp"

namespace fs = ghc::filesystem;

static bool hasBeenInterrupted = false;

#ifdef SFML_SYSTEM_LINUX

#include <stdio.h>
#include <signal.h>

static void sigintHandler(int) {
	signal(SIGINT, sigintHandler);
	hasBeenInterrupted = true;
}

#endif // SFML_SYSTEM_LINUX

ServerApplication::ServerApplication(int argc, char **argv) : m_argumentParser(argc, argv) {
#ifdef SFML_SYSTEM_LINUX
	signal(SIGINT, sigintHandler);
#endif

	m_worldName = "_server";
}

ServerApplication::ServerApplication(EventHandler &eventHandler) {
	m_eventHandler = &eventHandler;
}

bool ServerApplication::init() {
	std::srand((unsigned int)std::time(nullptr));

	gk::LoggerHandler::setInstance(m_loggerHandler);
	m_loggerHandler.setName("server");

	fs::create_directory("logs");
	m_loggerHandler.openLogFile("logs/openminer_server.log");

	BlockGeometry::initOrientation();

	m_argumentParser.addArgument("port", {"-p", "--port", "Select the port to use.", "port"});
	m_argumentParser.addArgument("world", {"-w", "--world", "Select a world to use.", "world"});
	m_argumentParser.addArgument("working-dir", {"-d", "--working-dir", "Change the working directory to <dir>.", "dir"});
	m_argumentParser.addArgument("log-level", {"-l", "--log-level", "Choose the log level (debug, info, warning, error)", "level"});

	m_argumentParser.parse();

	if (m_argumentParser.getArgument("help").isFound)
		return false;

	if (m_argumentParser.getArgument("working-dir").isFound) {
		try {
			fs::current_path(m_argumentParser.getArgument("working-dir").parameter);
		}
		catch (fs::filesystem_error &e){
			gkError() << "Failed to change working directory:" << e.what();
			return false;
		}
	}

	if (m_argumentParser.getArgument("port").isFound)
		m_port = (u16)std::stoi(m_argumentParser.getArgument("port").parameter);

	if (m_argumentParser.getArgument("world").isFound)
		m_worldName = m_argumentParser.getArgument("world").parameter;

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

	ServerConfig::loadConfigFromFile("config/server.lua");

	m_server.init(m_port);
	m_server.setRunning(true);

	Registry::setInstance(m_registry);

	BlockState &blockState = m_registry.registerBlock<ServerBlock>(BLOCK_AIR).getState(0);
	blockState.label("Air");
	blockState.isOpaque(false);
	blockState.isCollidable(false);
	m_registry.registerItem<Item>({}, BLOCK_AIR, "Air").setIsBlock(true);

	m_modLoader.loadMods();

	m_serverCommandHandler.setupCallbacks();

	m_worldController.setServer(m_serverCommandHandler);
	m_worldController.init(m_players, m_seed);

	m_scriptEngine.luaCore().setRegistry(&m_registry);

	gkInfo() << ("Server is running on localhost:" + std::to_string(m_server.port())).c_str();

	if (!m_worldName.empty())
		m_worldController.load(m_worldName);

	if (m_eventHandler)
		m_eventHandler->emplaceEvent<ServerOnlineEvent>(true, m_server.port());

	return true;
}

int ServerApplication::run(bool isProtected) {
	bool isInitSuccessful = false;
	if (isProtected) {
		try {
			if ((isInitSuccessful = init()))
				mainLoop();
		}
		catch(const gk::Exception &e) {
			if (m_eventHandler)
				m_eventHandler->emplaceEvent<ServerOnlineEvent>(false, (u16)0);

			gkError() << "Fatal error" << e.what();

			ServerConfig::saveConfigToFile("config/server.lua");
			ServerConfig::options.clear();

			m_serverCommandHandler.sendServerClosed(std::string("Server error ") + e.what());

			if (!m_server.isSingleplayer()) // Needed to prevent a crash when using '/stop'
				m_registry.clear();

			m_worldController.clearEntities();

			return 1;
		}
	}
	else {
		if ((isInitSuccessful = init()))
			mainLoop();
	}

	if (isInitSuccessful) {
		gkInfo() << "Stopping server...";

		if (!m_worldName.empty()) {
			m_worldController.save(m_worldName);
		}

		ServerConfig::saveConfigToFile("config/server.lua");
		ServerConfig::options.clear();

		m_serverCommandHandler.sendServerClosed("Server closed.");

		m_server.disconnectAllClients();

		m_registry.clear();
		m_worldController.clearEntities();

		return 0;
	}
	else
		return 1;
}

void ServerApplication::update() {
	static u64 lastTick = m_clock.getTicks() / 50;
	bool doTick = false;
	if (lastTick < m_clock.getTicks() / 50) {
		lastTick = m_clock.getTicks() / 50;
		doTick = true;

		GameTime::incrementTicks();

		m_serverCommandHandler.sendServerTick();
	}

	m_worldController.update(doTick);

	if (m_clock.getTicks() % 100 < 10) {
		for (auto &it : m_players) {
			if (it.second.isOnline()) {
				m_serverCommandHandler.sendPlayerPosUpdate(it.second.clientID());
				m_serverCommandHandler.sendPlayerRotUpdate(it.second.clientID());
			}
		}
	}
}

void ServerApplication::mainLoop() {
	while (!hasBeenInterrupted && m_server.isRunning()) {
		m_server.handleGameEvents();

		m_clock.updateGame([this] {
			update();
		});

		m_clock.waitForNextFrame();
	}
}

