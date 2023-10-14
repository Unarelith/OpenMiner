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

ServerConnectState::ServerConnectState(gk::ApplicationState *parent) : InterfaceState(parent) {
	m_background.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_filter.setFillColor(gk::Color::fromRGBA32(0, 0, 0, 192));

	m_title.setScale(Config::guiScale, Config::guiScale);
	m_title.setString("Play Multiplayer");
	m_title.updateVertexBuffer();

	m_usernameInput.setString(Config::defaultUsername);
	m_usernameInput.setCharacterLimit(20);
	m_usernameInput.setBackgroundSize(150, 20);
	m_usernameInput.setBackgroundOutline(1, gk::Color::White);
	m_usernameInput.setPadding(5, 6);
	m_usernameInput.setScale(Config::guiScale, Config::guiScale);
	m_usernameInput.setPlaceholder("Username");
	m_usernameInput.setFocus(false);

	m_addressInput.setString(Config::defaultServerAddress);
	m_addressInput.setCharacterLimit(15 + 1 + 6);
	m_addressInput.setBackgroundSize(150, 20);
	m_addressInput.setBackgroundOutline(1, gk::Color::White);
	m_addressInput.setPadding(5, 6);
	m_addressInput.setScale(Config::guiScale, Config::guiScale);
	m_addressInput.setPlaceholder("Server address");
	m_addressInput.setFocus(false);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget.addButton("Join Server", [this](TextButton &) {
		size_t sep = m_addressInput.string().find_first_of(':');
		std::string host = m_addressInput.string().substr(0, sep);

		bool isPortInvalid = false;
		int port = 0;
		if (sep != std::string::npos) {
			try {
				port = std::stoi(m_addressInput.string().substr(sep + 1));
			}
			catch (std::invalid_argument &) {
				isPortInvalid = true;
			}
		}

		std::string username = m_usernameInput.string();

		if (port == 0 || isPortInvalid) {
			m_errorText.setString("Error: Invalid server address");
			m_errorText.updateVertexBuffer();
			updateWidgetPosition();
		}
		else if (!gk::regexMatch(username, "^[A-Za-z0-9_]+$")) {
			m_errorText.setString("Error: Invalid username");
			m_errorText.updateVertexBuffer();
			updateWidgetPosition();
		}
		else {
			auto &game = m_stateStack->push<GameState>();
			auto &serverLoadingState = m_stateStack->push<ServerLoadingState>(game, true, host, port, this);
			serverLoadingState.setUsername(username);
			Config::defaultUsername = username;
			Config::defaultServerAddress = m_addressInput.string();
		}
	});

	m_menuWidget.addButton("Cancel", [this](TextButton &) {
		m_stateStack->pop();
	});

	m_errorText.setColor(gk::Color::Red);
	m_errorText.setScale(Config::guiScale, Config::guiScale);

	updateWidgetPosition();
}

void ServerConnectState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_usernameInput.onEvent(event);
		m_addressInput.onEvent(event);

		m_menuWidget.onEvent(event);
	}
}

void ServerConnectState::update() {
}

void ServerConnectState::updateWidgetPosition() {
	m_background.setPosRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_background.setClipRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_filter.setSize(Config::screenWidth, Config::screenHeight);

	m_title.setPosition(
		Config::screenWidth / 2.0f - float(m_title.getSize().x * Config::guiScale) / 2.0f,
		12.5f * Config::guiScale - float(m_title.getSize().y * Config::guiScale) / 2.0f
	);

	m_usernameInput.setPosition(
		Config::screenWidth / 2.0f - float(m_usernameInput.getBackgroundSize().x * Config::guiScale) / 2.0f,
		Config::screenHeight / 2.0f - float(m_usernameInput.getBackgroundSize().y * Config::guiScale) / 2.0f - 30 * Config::guiScale
	);

	m_addressInput.setPosition(
		Config::screenWidth / 2.0f - float(m_addressInput.getBackgroundSize().x * Config::guiScale) / 2.0f,
		Config::screenHeight / 2.0f - float(m_addressInput.getBackgroundSize().y * Config::guiScale) / 2.0f
	);

	m_menuWidget.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2.f,
		Config::screenHeight - 0.10f * Config::screenHeight * Config::guiScale
	);

	m_errorText.setPosition(
		Config::screenWidth / 2.0f - float(m_errorText.getSize().x * Config::guiScale) / 2.0f,
		Config::screenHeight / 2.0f + 30 * Config::guiScale
	);
}

void ServerConnectState::draw(gk::RenderTarget &target, RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter, states);

		target.draw(m_title, states);

		target.draw(m_usernameInput, states);
		target.draw(m_addressInput, states);

		target.draw(m_menuWidget, states);

		if (!m_errorText.string().empty())
			target.draw(m_errorText, states);
	}
}

