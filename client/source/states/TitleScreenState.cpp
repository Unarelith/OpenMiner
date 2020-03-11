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
#include <gk/core/ApplicationStateStack.hpp>

#include "Config.hpp"
#include "GameState.hpp"
#include "ServerConnectState.hpp"
#include "ServerLoadingState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"

#include "ServerApplication.hpp"

TitleScreenState::TitleScreenState(u16 port) : m_port(port) {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale, 1);

	m_menuWidget.addButton("Singleplayer", [this] (TextButton &) {
		startSingleplayer(true);
	});

	m_menuWidget.addButton("Multiplayer", [this] (TextButton &) {
		m_stateStack->push<ServerConnectState>(this);
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>(this);
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		m_stateStack->pop();
	});

	centerBackground();
}

TitleScreenState::~TitleScreenState() {
	if (m_thread.joinable())
		m_thread.join();
}

void TitleScreenState::centerBackground() {
	m_background.setPosition(Config::screenWidth / 2.0 - m_background.width() / 2.0, Config::screenHeight / 2.0 - m_background.height() / 2.0);
}

void TitleScreenState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		centerBackground();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
	}
}

void TitleScreenState::update() {
}

void TitleScreenState::startSingleplayer(bool showLoadingState) {
	m_thread = std::thread([this] () {
		ServerApplication app;
		app.setSingleplayer(true);
		app.setPort(m_port);
		app.run();
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	auto &game = m_stateStack->push<GameState>();
	game.setSingleplayer(true);
	game.connect("localhost", m_port);

	m_stateStack->push<ServerLoadingState>(game, showLoadingState, this);
}

void TitleScreenState::startMultiplayer(const std::string &host) {
	auto &game = m_stateStack->push<GameState>();
	game.connect(host, m_port);

	m_stateStack->push<ServerLoadingState>(game, false, this);
}

void TitleScreenState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_background, states);

	if (&m_stateStack->top() == this) {
		target.draw(m_menuWidget, states);
	}
}

