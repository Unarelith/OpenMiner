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
#include "ServerApplication.hpp"
#include "ServerBlock.hpp"

using namespace std::literals::string_literals;

ServerApplication::ServerApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	m_loadSDL = false;
}

void ServerApplication::init() {
	m_argumentParser.addArgument("port", {"-p", "--port", true});

	gk::CoreApplication::init();

	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	Registry::setInstance(m_registry);

	// The id "_:air" is used in CraftingRecipe, update it there if it changes
	m_registry.registerBlock<ServerBlock>({}, "_:air", "Air");
	m_registry.registerItem({}, "_:air", "Air").setIsBlock(true);

	m_scriptEngine.init();
	// m_luaCore.setPlayer(m_player); // FIXME
	m_luaCore.setWorld(m_world);

	try {
		m_scriptEngine.lua()["openminer"] = &m_luaCore;
		// m_scriptEngine.lua().script("init()");
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	m_scriptEngine.lua().safe_script_file("mods/default/init.lua");

	m_server.init(m_port);
	m_server.setRunning(true);

	m_serverCommandHandler.setupCallbacks();

	m_world.setServer(&m_serverCommandHandler);
}

void ServerApplication::update() {
	m_world.update(m_players);

	if (gk::GameClock::getTicks() % 1000 < 10) {
		for (auto &it : m_players) {
			m_serverCommandHandler.sendPlayerPosUpdate(it.first, it.second);
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

