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

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "GameState.hpp"
#include "ServerLoadingState.hpp"
#include "TextureAtlas.hpp"

ServerLoadingState::ServerLoadingState(GameState &game, bool showLoadingState, gk::ApplicationState *parent)
	: InterfaceState(parent), m_game(game), m_showLoadingState(showLoadingState)
{
	m_text.setText("Loading world...");
	m_text.setColor(gk::Color::White);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 2, Config::guiScale * 2);

	centerText();

	// FIXME: SFML_RAW_MOUSE
	// gk::Mouse::setCursorVisible(true);
	// gk::Mouse::setCursorGrabbed(false);
}

void ServerLoadingState::centerText() {
	m_text.setPosition(Config::screenWidth  / 2 - m_text.getSize().x * Config::guiScale * 2 / 2,
	                   Config::screenHeight / 2 - m_text.getSize().y * Config::guiScale * 2 / 2);
}

void ServerLoadingState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		centerText();
	}
}

void ServerLoadingState::update() {
	m_game.client().update();

	if (m_game.clientCommandHandler().isRegistryInitialized()) {
		if (m_game.textureAtlas().isReady() && (m_hasBeenRendered || !m_showLoadingState)) {
			if (m_showLoadingState)
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

			m_game.world().updateSky(0);

			m_stateStack->pop();

			gk::Mouse::setCursorVisible(false);
			gk::Mouse::setCursorGrabbed(true);
		}
		else if (!m_game.textureAtlas().isReady()) {
			try {
				m_game.textureAtlas().loadFromRegistry(m_texturePack);
			}
			catch (gk::Exception &e) {
				m_game.client().disconnect();
				throw e;
			}
		}
	}
}

void ServerLoadingState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (m_showLoadingState) {
		prepareDraw(target, states);

		target.draw(m_text, states);

		m_hasBeenRendered = true;
	}
}

