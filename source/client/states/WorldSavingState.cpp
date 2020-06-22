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

#include "Client.hpp"
#include "Config.hpp"
#include "TitleScreenState.hpp"
#include "WorldSavingState.hpp"

WorldSavingState::WorldSavingState(Client &client, bool closeClient, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client)
{
	m_closeClient = closeClient;

	m_backgroundFilter.setFillColor(gk::Color{0, 0, 0, 127});

	m_text.setString("Saving world...");
	m_text.setColor(gk::Color::White);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 2, Config::guiScale * 2);

	m_client.setCommandCallback(Network::Command::ServerClosed, [&](sf::Packet &) {
		m_isWorldSaved = true;
	});

	updateWidgetPosition();
}

void WorldSavingState::onEvent(const sf::Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == sf::Event::Resized) {
		updateWidgetPosition();
	}
}

void WorldSavingState::update() {
	if (m_hasBeenRendered || m_isWorldSaved) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_client.disconnect();

		m_stateStack->clear();

		if (!m_closeClient)
			m_stateStack->push<TitleScreenState>().setTexturePack(m_client.texturePack());
	}
	else {
		m_client.update();
	}
}

void WorldSavingState::updateWidgetPosition() {
	m_text.setPosition(Config::screenWidth  / 2 - m_text.getSize().x * Config::guiScale * 2 / 2,
	                   Config::screenHeight / 2 - m_text.getSize().y * Config::guiScale * 2 / 2);

	m_background.setPosition(Config::screenWidth / 2.0 - m_background.width() / 2.0, Config::screenHeight / 2.0 - m_background.height() / 2.0);
}

void WorldSavingState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_backgroundFilter, states);

		target.draw(m_text, states);

		m_hasBeenRendered = true;
	}
}

