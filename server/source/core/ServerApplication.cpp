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

using namespace std::literals::string_literals;

ServerApplication::ServerApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	if (argc == 3 && argv[1] == "--port"s) {
		m_port = std::stoi(argv[2]);
	}

	m_loadSDL = false;
}

void ServerApplication::init() {
	gk::CoreApplication::init();

	Registry::setInstance(m_registry);

	m_scriptEngine.init();
	// m_luaCore.setPlayer(m_player);
	// m_luaCore.setWorld(m_world);

	try {
		m_scriptEngine.lua()["openminer"] = &m_luaCore;
		// FIXME
		// m_scriptEngine.lua().script("init()");
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	m_server.init(m_port);
	m_server.setRunning(true);

	m_server.setConnectionCallback([this](Client &client) {
		sf::Packet packet;
		packet << Network::Command::RegistryData;
		m_registry.serialize(packet);
		client.tcpSocket->send(packet);

		m_world.sendWorldData(client);
	});

	// m_server.setCommandCallback(Network::Command::ClientSettings, [](sf::Packet &packet) {
	// 	packet >> Config::renderDistance;
	// });

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](Client &client, sf::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		m_world.sendRequestedData(client, cx, cy, cz);
	});

	m_server.setCommandCallback(Network::Command::PlayerPlaceBlock, [this](Client &, sf::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;
		m_world.setBlock(x, y, z, block);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << block;
		m_server.sendToAllClients(answer);
	});

	m_server.setCommandCallback(Network::Command::PlayerDigBlock, [this](Client &, sf::Packet &packet) {
		s32 x, y, z;
		packet >> x >> y >> z;
		m_world.setBlock(x, y, z, 0);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << u32(0);
		m_server.sendToAllClients(answer);
	});
}

void ServerApplication::mainLoop() {
	while (m_server.isRunning()) {
		m_server.handleGameEvents();

		m_server.handleKeyState();

		m_clock.updateGame([&] {
			m_world.update(m_server);
		});

		m_clock.waitForNextFrame();
	}
}

