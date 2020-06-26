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
#include <gk/core/Mouse.hpp>

#include "Config.hpp"
#include "ConnectionErrorState.hpp"
#include "GameState.hpp"
#include "ServerLoadingState.hpp"
#include "TitleScreenState.hpp"

ConnectionErrorState::ConnectionErrorState(const std::string &error, const std::string &host, u16 port, const std::string &texturePack, gk::ApplicationState *parent) : InterfaceState(parent) {
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);

	m_host = host;
	m_port = port;

	m_texturePack = texturePack;

	m_text.setString(error);
	m_text.setColor(gk::Color::Red);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 1.5, Config::guiScale * 1.5);

	m_reconnectButton.setText("Reconnect");
	m_reconnectButton.setScale(Config::guiScale, Config::guiScale);
	m_reconnectButton.setCallback([this](TextButton &) {
		m_stateStack->pop();

		auto &game = m_stateStack->push<GameState>();
		auto &serverLoadingState = m_stateStack->push<ServerLoadingState>(game, true, m_host, m_port, m_parent);
		serverLoadingState.setTexturePack(m_texturePack);
		serverLoadingState.setUsername(Config::defaultUsername);
	});

	m_cancelButton.setText("Cancel");
	m_cancelButton.setScale(Config::guiScale, Config::guiScale);
	m_cancelButton.setCallback([this](TextButton &) {
		if (m_stateStack->size() > 1) {
			m_stateStack->pop();
		}
		else {
			m_stateStack->pop();
			m_stateStack->push<TitleScreenState>(m_port).setTexturePack(m_texturePack);
		}
	});

	updateWidgetPosition();
}

void ConnectionErrorState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		updateWidgetPosition();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_reconnectButton.onEvent(event);
		m_cancelButton.onEvent(event);
	}
}

void ConnectionErrorState::updateWidgetPosition() {
	m_text.setPosition(Config::screenWidth  / 2 - m_text.getSize().x * Config::guiScale * 1.5 / 2,
	                   Config::screenHeight / 2 - m_text.getSize().y * Config::guiScale * 1.5 / 2);

	m_reconnectButton.setPosition(Config::screenWidth / 2.0f - m_reconnectButton.getGlobalBounds().sizeX / 2.0f, Config::screenHeight - 340);
	m_cancelButton.setPosition(Config::screenWidth / 2.0f - m_cancelButton.getGlobalBounds().sizeX / 2.0f, Config::screenHeight - 261);
}

void ConnectionErrorState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);

		target.draw(m_text, states);

		target.draw(m_reconnectButton, states);
		target.draw(m_cancelButton, states);
	}
}

