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

void ServerApplication::init() {
	m_server.init();
	m_server.setRunning(true);
	m_server.setGameStarted(false);

	gk::CoreApplication::init();

	Registry::setInstance(m_registry);

	// m_scriptEngine.init();
}

void ServerApplication::mainLoop() {
	while (m_server.isRunning()) {
		handleEvents();

		m_server.handleGameEvents();

		if (m_server.hasGameStarted()) {
			m_server.handleKeyState();

			m_clock.updateGame([&] {
			});

			m_clock.waitForNextFrame();
		}
	}
}

