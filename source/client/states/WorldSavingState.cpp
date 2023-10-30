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
#include "ApplicationStateStack.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "TitleScreenState.hpp"
#include "WorldSavingState.hpp"

WorldSavingState::WorldSavingState(Client &client, bool closeClient, DrawableState *parent)
	: InterfaceState(parent), m_client(client)
{
	m_closeClient = closeClient;

	m_background.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_filter.setFillColor(Color::fromRGBA32(0, 0, 0, 192));

	m_text.setString("Saving world...");
	m_text.setColor(Color::White);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_client.setCommandCallback(Network::Command::ServerClosed, [&](sf::Packet &) {
		m_isWorldSaved = true;
	});

	updateWidgetPosition();
}

void WorldSavingState::update() {
	if (m_hasBeenRendered || m_isWorldSaved) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_client.disconnect();

		m_stateStack->clear();

		if (!m_closeClient)
			m_stateStack->push<TitleScreenState>();
	}
	else {
		m_client.update();
	}
}

void WorldSavingState::updateWidgetPosition() {
	m_background.setPosRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_background.setClipRect(0, 0,
		u16(Config::screenWidth / m_background.getScale().x),
		u16(Config::screenHeight / m_background.getScale().y)
	);

	m_filter.setSize(Config::screenWidth, Config::screenHeight);

	m_text.setPosition(Config::screenWidth  / 2.f - float(m_text.getSize().x * Config::guiScale) * 2.f / 2.f,
	                   Config::screenHeight / 2.f - float(m_text.getSize().y * Config::guiScale) * 2.f / 2.f);
}

void WorldSavingState::draw(RenderTarget &target, RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter, states);

		target.draw(m_text, states);

		m_hasBeenRendered = true;
	}
}

