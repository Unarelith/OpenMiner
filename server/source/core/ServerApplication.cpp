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

ServerApplication::ServerApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	m_loadSDL = false;
}

void ServerApplication::init() {
	gk::CoreApplication::init();

	m_server.init();
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
		});

		m_clock.waitForNextFrame();
	}
}

