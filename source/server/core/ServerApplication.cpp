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

ServerApplication::ServerApplication(gk::EventHandler &eventHandler) {
	m_eventHandler = &eventHandler;
}

bool ServerApplication::init() {
	std::srand(std::time(nullptr));

	m_loggerHandler.setName("server");
	gk::LoggerHandler::setInstance(m_loggerHandler);

	BlockGeometry::initOrientation();

	m_argumentParser.addArgument("port", {"-p", "--port", "Select the port to use.", "port"});
	m_argumentParser.addArgument("save", {"-s", "--save", "Select a save file to use.", "save"});
	m_argumentParser.addArgument("working-dir", {"-w", "--working-dir", "Change the working direction to <dir>.", "dir"});

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
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	if (m_argumentParser.getArgument("save").isFound)
		m_worldName = m_argumentParser.getArgument("save").parameter;

	ServerConfig::loadConfigFromFile("config/server.lua");

	m_server.init(m_port);
	m_server.setRunning(true);

	Registry::setInstance(m_registry);

	// The id "_:air" is used in CraftingRecipe, update it there if it changes
	m_registry.registerBlock<ServerBlock>({}, "_:air", "Air");
	m_registry.registerItem({}, "_:air", "Air").setIsBlock(true);

	m_modLoader.loadMods();

	m_serverCommandHandler.setupCallbacks();

	m_worldController.setServer(m_serverCommandHandler);
	m_worldController.init(m_players);

	m_scriptEngine.luaCore().setRegistry(&m_registry);

	gkInfo() << ("Server is running on localhost:" + std::to_string(m_server.port())).c_str();

	if (m_eventHandler)
		m_eventHandler->emplaceEvent<ServerOnlineEvent>(true, m_server.port());

	if (!m_worldName.empty())
		m_worldController.load(m_worldName);

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
				m_eventHandler->emplaceEvent<ServerOnlineEvent>(false, 0);

			gkError() << "Fatal error" << e.what();

			ServerConfig::saveConfigToFile("config/server.lua");
			ServerConfig::options.clear();

			m_serverCommandHandler.sendServerClosed(std::string("Server error ") + e.what());

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

		m_registry.clear();
		m_worldController.clearEntities();
	}

	return 0;
}

void ServerApplication::update() {
	m_worldController.update();

	if (m_clock.getTicks() % 100 < 10) {
		for (auto &it : m_players) {
			m_serverCommandHandler.sendPlayerPosUpdate(it.first);
			m_serverCommandHandler.sendPlayerRotUpdate(it.first);
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

