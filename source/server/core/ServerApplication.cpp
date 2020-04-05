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
#include "ServerApplication.hpp"
#include "ServerBlock.hpp"

namespace fs = ghc::filesystem;

ServerApplication::ServerApplication(int argc, char **argv) : m_argumentParser(argc, argv) {
}

ServerApplication::ServerApplication(gk::EventHandler &eventHandler) {
	m_eventHandler = &eventHandler;
}

void ServerApplication::init() {
	std::srand(std::time(nullptr));

	BlockGeometry::initOrientation();

	m_argumentParser.addArgument("port", {"-p", "--port", "Select the port to use.", "port"});
	m_argumentParser.addArgument("working-dir", {"-w", "--working-dir", "Change the working direction to <dir>.", "dir"});

	m_argumentParser.parse();

	if (m_argumentParser.getArgument("help").isFound)
		return;

	if (m_argumentParser.getArgument("working-dir").isFound)
		fs::current_path(m_argumentParser.getArgument("working-dir").parameter);

	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

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
}

int ServerApplication::run(bool isProtected) {
	if (isProtected) {
		try {
			init();
			mainLoop();
		}
		catch(const gk::Exception &e) {
			if (m_eventHandler)
				m_eventHandler->emplaceEvent<ServerOnlineEvent>(false);

			std::cerr << "Fatal error " << e.what() << std::endl;
			return 1;
		}
	}
	else {
		init();
		mainLoop();
	}

	return 0;
}

void ServerApplication::update() {
	m_worldController.update();

	if (m_clock.getTicks() % 100 < 10) {
		for (auto &it : m_players) {
			m_serverCommandHandler.sendPlayerPosUpdate(it.first);
		}
	}
}

void ServerApplication::mainLoop() {
	while (m_server.isRunning()) {
		m_server.handleGameEvents();

		m_server.handleKeyState();

		m_clock.updateGame([this] {
			update();
		});

		m_clock.waitForNextFrame();
	}
}

