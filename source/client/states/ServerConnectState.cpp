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
	m_usernameInput.setString("");
	m_usernameInput.setCharacterLimit(20);
	m_usernameInput.setBackgroundSize(150, 20);
	m_usernameInput.setBackgroundOutline(1, gk::Color::White);
	m_usernameInput.setPadding(5, 6);
	m_usernameInput.setScale(Config::guiScale, Config::guiScale);
	m_usernameInput.setPlaceholder("Username");
	m_usernameInput.setFocus(false);

	m_addressInput.setString("localhost:4242");
	m_addressInput.setCharacterLimit(15 + 1 + 6);
	m_addressInput.setBackgroundSize(150, 20);
	m_addressInput.setBackgroundOutline(1, gk::Color::White);
	m_addressInput.setPadding(5, 6);
	m_addressInput.setScale(Config::guiScale, Config::guiScale);
	m_addressInput.setPlaceholder("Server address");
	m_addressInput.setFocus(false);

	m_connectButton.setText("Connect");
	m_connectButton.setScale(Config::guiScale, Config::guiScale);
	m_connectButton.setCallback([this](TextButton &) {
		size_t sep = m_addressInput.string().find_first_of(':');
		std::string host = m_addressInput.string().substr(0, sep);

		bool isPortInvalid = false;
		int port = 0;
		if (sep != std::string::npos) {
			try {
				port = std::stoi(m_addressInput.string().substr(sep + 1));
			}
			catch (const std::invalid_argument &e) {
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
			serverLoadingState.setTexturePack(m_texturePack);
			serverLoadingState.setUsername(username);
		}
	});

	m_cancelButton.setText("Cancel");
	m_cancelButton.setScale(Config::guiScale, Config::guiScale);
	m_cancelButton.setCallback([this](TextButton &) {
		m_stateStack->pop();
	});

	m_errorText.setColor(gk::Color::Red);
	m_errorText.setScale(Config::guiScale, Config::guiScale);

	updateWidgetPosition();
}

void ServerConnectState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		updateWidgetPosition();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_usernameInput.onEvent(event);
		m_addressInput.onEvent(event);

		m_connectButton.onEvent(event);
		m_cancelButton.onEvent(event);
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		gk::FloatRect usernameInputRect{
			m_usernameInput.getPosition().x,
			m_usernameInput.getPosition().y,
			m_usernameInput.getBackgroundSize().x * m_usernameInput.getScale().x,
			m_usernameInput.getBackgroundSize().y * m_usernameInput.getScale().y
		};

		gk::FloatRect addressInputRect{
			m_addressInput.getPosition().x,
			m_addressInput.getPosition().y,
			m_addressInput.getBackgroundSize().x * m_addressInput.getScale().x,
			m_addressInput.getBackgroundSize().y * m_addressInput.getScale().y
		};

		if (usernameInputRect.contains(event.mouseButton.x, event.mouseButton.y)) {
			m_usernameInput.setFocus(true);
			m_addressInput.setFocus(false);
		}
		else if (addressInputRect.contains(event.mouseButton.x, event.mouseButton.y)) {
			m_usernameInput.setFocus(false);
			m_addressInput.setFocus(true);
		}
	}
}

void ServerConnectState::update() {
}

void ServerConnectState::updateWidgetPosition() {
	m_usernameInput.setPosition(
		Config::screenWidth / 2.0f - m_usernameInput.getBackgroundSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - m_usernameInput.getBackgroundSize().y * Config::guiScale / 2.0f - 25 * Config::guiScale
	);

	m_addressInput.setPosition(
		Config::screenWidth / 2.0f - m_addressInput.getBackgroundSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f - m_addressInput.getBackgroundSize().y * Config::guiScale / 2.0f
	);

	m_connectButton.setPosition(Config::screenWidth / 2.0f - m_connectButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 340);
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2, Config::screenHeight - 261);

	m_errorText.setPosition(
		Config::screenWidth / 2.0f - m_errorText.getSize().x * Config::guiScale / 2.0f,
		Config::screenHeight / 2.0f + 30 * Config::guiScale
	);
}

void ServerConnectState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_usernameInput, states);
		target.draw(m_addressInput, states);

		target.draw(m_connectButton, states);
		target.draw(m_cancelButton, states);

		if (!m_errorText.string().empty())
			target.draw(m_errorText, states);
	}
}

