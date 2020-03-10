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
#include <experimental/filesystem>

#include "ServerApplication.hpp"
#include "ServerBlock.hpp"

namespace fs = std::experimental::filesystem;

ServerApplication::ServerApplication(int argc, char **argv) : m_argumentParser(argc, argv) {
	std::srand(std::time(nullptr));

	m_argumentParser.addArgument("port", {"-p", "--port", true});
	m_argumentParser.parse();
}

void ServerApplication::init() {
	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	Registry::setInstance(m_registry);

	// The id "_:air" is used in CraftingRecipe, update it there if it changes
	m_registry.registerBlock<ServerBlock>({}, "_:air", "Air");
	m_registry.registerItem({}, "_:air", "Air").setIsBlock(true);

	loadMods();

	m_server.init(m_port);
	m_server.setRunning(true);

	m_serverCommandHandler.setupCallbacks();

	m_worldController.setServer(m_serverCommandHandler);
	m_worldController.init();

	m_scriptEngine.luaCore().setRegistry(&m_registry);

	std::cout << "Server is running on localhost:" << m_port << std::endl;
}

int ServerApplication::run(bool isProtected) {
	if (isProtected) {
		try {
			init();
			mainLoop();
		}
		catch(const gk::Exception &e) {
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

	if (gk::GameClock::getTicks() % 100 < 10) {
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

void ServerApplication::loadMods() {
	m_scriptEngine.init();

	try {
		fs::directory_iterator dir("mods/");
		for (const auto &entry : dir) {
			if (fs::exists(entry.path().string() + "/init.lua")) {
				m_scriptEngine.lua().safe_script_file(entry.path().string() + "/init.lua");
				std::cout << "Mod '" + entry.path().filename().string() + "' loaded" << std::endl;
			}
			else
				DEBUG("WARNING: The mod '" + entry.path().filename().string() + "' doesn't contain an 'init.lua' file.");
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
		return;
	}
}

