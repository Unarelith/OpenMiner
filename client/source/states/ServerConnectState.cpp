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

#include "GameState.hpp"
#include "ServerConnectState.hpp"
#include "ServerLoadingState.hpp"

ServerConnectState::ServerConnectState() {
	m_textInput.setContent("localhost:4242");
	m_textInput.setCharacterLimit(15 + 1 + 6);
	m_textInput.setSize(400, 54);
	m_textInput.setPosition(Config::screenWidth / 2.0f - m_textInput.getSize().x / 2, Config::screenHeight / 2.0f - m_textInput.getSize().y / 2);
	m_textInput.setCursor("_");

	m_connectButton.setText("Connect");
	m_connectButton.setPosition(Config::screenWidth / 2.0f - m_connectButton.getGlobalBounds().sizeX * Config::guiScale / 2.0f, Config::screenHeight - 340);
	m_connectButton.setScale(Config::guiScale, Config::guiScale, 1);
	m_connectButton.setCallback([this](TextButton &) {
		size_t sep = m_textInput.content().find_first_of(':');

		std::string host = m_textInput.content().substr(0, sep);

		int port = 0;
		try {
			port = std::stoi(m_textInput.content().substr(sep + 1));
		}
		catch (const std::invalid_argument &e) {
			std::cerr << "Error: Invalid server address." << std::endl;
		}

		m_stateStack->push<GameState>(host, port);

		// auto &game = m_stateStack->push<GameState>(host, port);
		// m_stateStack->push<ServerLoadingState>(game);
	});

	m_cancelButton.setText("Cancel");
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX * Config::guiScale / 2.0f, Config::screenHeight - 261);
	m_cancelButton.setScale(Config::guiScale, Config::guiScale, 1);
	m_cancelButton.setCallback([this](TextButton &) {
		m_stateStack->pop();
	});
}

void ServerConnectState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		m_textInput.setPosition(Config::screenWidth / 2.0f - m_textInput.getSize().x / 2, Config::screenHeight / 2.0f - m_textInput.getSize().y / 2);
		m_connectButton.setPosition(Config::screenWidth / 2.0f - m_connectButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 340);
		m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 261);
	}

	m_textInput.onEvent(event);

	m_connectButton.onEvent(event);
	m_cancelButton.onEvent(event);
}

void ServerConnectState::update() {
}

void ServerConnectState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_textInput, states);

	target.draw(m_connectButton, states);
	target.draw(m_cancelButton, states);
}

