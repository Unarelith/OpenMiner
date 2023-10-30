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
#include <thread>

#include <gk/resource/ResourceHandler.hpp>

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "ConnectionErrorState.hpp"
#include "GameConfig.hpp"
#include "GameState.hpp"
#include "Events.hpp"
#include "Mouse.hpp"
#include "ServerLoadingState.hpp"
#include "TextureAtlas.hpp"

ServerLoadingState::ServerLoadingState(GameState &game, bool showLoadingState, const std::string &host, u16 port, ApplicationState *parent)
	: InterfaceState(parent), m_game(game), m_showLoadingState(showLoadingState)
{
	m_background.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_filter.setFillColor(Color::fromRGBA32(0, 0, 0, 192));

	m_text.setString("Loading world...");
	m_text.setColor(Color::White);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	updateWidgetPosition();

	m_host = host;
	m_port = port;

	Mouse::setCursorVisible(true);
	Mouse::setCursorGrabbed(false);

	m_game.textureAtlas().clear();
}

void ServerLoadingState::init() {
	m_eventHandler->addListener<ServerOnlineEvent>(&ServerLoadingState::onServerOnlineEvent, this);
}

void ServerLoadingState::update() {
	if (!m_isConnected && (m_isServerOnline || m_port != 0)) {
		try {
			m_game.connect(m_host, m_port, m_username);
			m_isConnected = true;
		}
		catch (ClientConnectException &e) {
			m_stateStack->pop(); // GameState
			m_stateStack->pop(); // ServerLoadingState

			m_stateStack->push<ConnectionErrorState>(e.what(), m_host, m_port, m_parent);
		}
	}

	if (m_isConnected)
		m_game.client().update();

	if (m_game.clientCommandHandler().isRegistryInitialized()) {
		if (m_game.textureAtlas().isReady() && (m_hasBeenRendered || !m_showLoadingState)) {
			m_game.world().changeDimension(m_game.player().dimension());

			if (m_showLoadingState)
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

			m_game.clientCommandHandler().sendPlayerReady();

			m_stateStack->pop();

			Mouse::setCursorVisible(false);
			Mouse::setCursorGrabbed(true);

			GameConfig::isGameRunning = true;
		}
		else if (!m_game.textureAtlas().isReady()) {
			try {
				m_game.textureAtlas().loadFromRegistry(Config::texturePack);
			}
			catch (gk::Exception &e) {
				m_game.client().disconnect();
				throw e;
			}
		}
	}
}

void ServerLoadingState::onServerOnlineEvent(const ServerOnlineEvent &event) {
	m_isServerOnline = event.isOnline;
	m_port = event.port;
}

void ServerLoadingState::updateWidgetPosition() {
	m_background.setPosRect(0.f, 0.f,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_background.setClipRect(0.f, 0.f,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_filter.setSize(Config::screenWidth, Config::screenHeight);

	m_text.setPosition(Config::screenWidth  / 2.f - float(m_text.getSize().x * Config::guiScale) * 2.f / 2.f,
	                   Config::screenHeight / 2.f - float(m_text.getSize().y * Config::guiScale) * 2.f / 2.f);
}

void ServerLoadingState::draw(RenderTarget &target, RenderStates states) const {
	if (m_showLoadingState) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter, states);

		target.draw(m_text, states);

		m_hasBeenRendered = true;
	}
}

