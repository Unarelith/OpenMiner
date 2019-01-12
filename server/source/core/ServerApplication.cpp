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
#include "Config.hpp"
#include "Network.hpp"
#include "ServerApplication.hpp"

ServerApplication::ServerApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	m_loadSDL = false;
}

void ServerApplication::init() {
	gk::CoreApplication::init();

	m_server.setConnectionCallback([](Client &client) {
		sf::Packet packet;
		packet << Network::Command::ChunkData;
		for (u64 i = 0 ; i < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH ; ++i)
			packet << u16(rand() % 2);

		client.tcpSocket->send(packet);
	});

	m_server.init(4242);
	m_server.setRunning(true);
	m_server.setGameStarted(false);

	Registry::setInstance(m_registry);

	// m_scriptEngine.init();
}

void ServerApplication::mainLoop() {
	while (m_server.isRunning()) {
		m_server.handleGameEvents();

		m_server.handleKeyState();

		m_clock.updateGame([&] {
			// TODO
		});

		m_clock.waitForNextFrame();
	}
}

