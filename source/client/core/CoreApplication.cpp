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
#include <ctime>

#include <gk/core/Exception.hpp>

#include "CoreApplication.hpp"
#include "Mouse.hpp"
#include "Platform.hpp"

bool CoreApplication::hasBeenInterrupted = false;

#ifdef OM_PLATFORM_LINUX

#include <stdio.h>
#include <signal.h>

static void sigintHandler(int) {
	signal(SIGINT, sigintHandler);
	CoreApplication::hasBeenInterrupted = true;
}

#endif // OM_PLATFORM_LINUX

CoreApplication::CoreApplication(int argc, char **argv) : m_argumentParser(argc, argv) {
#ifdef OM_PLATFORM_LINUX
	signal(SIGINT, sigintHandler);
#endif
}

bool CoreApplication::init() {
	std::srand((unsigned int)std::time(nullptr));

	Mouse::setWindow(&m_window);

	ApplicationStateStack::setInstance(m_stateStack);
	gk::GameClock::setInstance(m_clock);
	gk::ResourceHandler::setInstance(m_resourceHandler);
	gk::LoggerHandler::setInstance(m_loggerHandler);

	m_stateStack.setEventHandler(m_eventHandler);

	// m_argumentParser.addArgument("mute", {"", "--mute", "Mute the program."});
	m_argumentParser.parse();
	// if (m_argumentParser.getArgument("mute").isFound)
	// 	AudioPlayer::setMuteState(true);
	// m_argumentParser.debug();

	return true;
}

int CoreApplication::run(bool isProtected) {
	bool isInitSuccessful = false;
	auto runGame = [&]() {
		if (m_loadSDL)
			m_sdlLoader.load();

		if ((isInitSuccessful = init()))
			mainLoop();
	};

	if (isProtected) {
		try {
			runGame();
		}
		catch(const gk::Exception &e) {
			gkError() << "Fatal error" << e.what();
			exit();
			return 1;
		}
		// catch(const std::exception &e) {
		// 	std::cerr << "Exception caught: " << e.what() << std::endl;
		// 	return 1;
		// }
		// catch(...) {
		// 	std::cerr << "Fatal error: Unknown error." << std::endl;
		// 	return 1;
		// }
	}
	else {
		runGame();
	}

	if (isInitSuccessful)
		onExit();

	exit();

	return !isInitSuccessful;
}

void CoreApplication::exit() {
	m_stateStack.clearAll();

	m_resourceHandler.clear();

	m_window.close();
}

void CoreApplication::createWindow(u16 width, u16 height, const std::string &title) {
	m_window.open(title, width, height);
}

void CoreApplication::handleEvents() {
	m_currentState = nullptr;
	if (!m_stateStack.empty())
		m_currentState = &m_stateStack.top();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		onEvent(event);
	}
}

void CoreApplication::onEvent(const SDL_Event &event) {
	m_window.onEvent(event);

	if (m_currentState && !m_stateStack.empty())
		m_currentState->onEvent(event);
}

