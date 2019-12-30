/*
 * =====================================================================================
 *
 *       Filename:  ServerApplication.cpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:41:15
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
	m_registry.registerBlock<ServerBlock>(0, "", "Air");
	m_registry.registerItem(0, "", "Air").setIsBlock(true);

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

	m_server.init(m_port);
	m_server.setRunning(true);

	m_serverCommandHandler.setupCallbacks();
}

void ServerApplication::update() {
	m_world.update(m_server, m_players);

	if (gk::GameClock::getTicks() % 1000 < 10) {
		for (auto &it : m_players) {
			sf::Packet packet;
			packet << Network::Command::PlayerPosUpdate;
			packet << it.first;
			packet << it.second.x() << it.second.y() << it.second.z();
			m_server.sendToAllClients(packet);
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

