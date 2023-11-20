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
#include "common/core/ApplicationStateStack.hpp"

#include "client/core/Config.hpp"
#include "client/core/Mouse.hpp"
#include "client/states/ConnectionErrorState.hpp"
#include "client/states/GameState.hpp"
#include "client/states/ServerLoadingState.hpp"
#include "client/states/TitleScreenState.hpp"

ConnectionErrorState::ConnectionErrorState(const std::string &error, const std::string &host, u16 port, ApplicationState *parent) : InterfaceState(parent) {
	Mouse::setCursorGrabbed(false);
	Mouse::setCursorVisible(true);

	m_host = host;
	m_port = port;

	m_background.setScale(Config::guiScale * 2.f, Config::guiScale * 2.f);

	m_filter.setFillColor(Color::fromRGBA32(0, 0, 0, 192));

	m_text.setString(error);
	m_text.setColor(Color::Red);
	m_text.updateVertexBuffer();
	m_text.setScale(Config::guiScale * 1.5f, Config::guiScale * 1.5f);
	m_text.setShadowEnabled(false);

	m_menuWidget.setScale(Config::guiScale, Config::guiScale);
	m_menuWidget.addButton("Reconnect", [this](TextButton &) {
		m_stateStack->pop();

		auto &game = m_stateStack->push<GameState>();
		auto &serverLoadingState = m_stateStack->push<ServerLoadingState>(game, true, m_host, m_port, m_parent);
		serverLoadingState.setUsername(Config::defaultUsername);
	});

	m_menuWidget.addButton("Cancel", [this](TextButton &) {
		if (m_stateStack->size() > 1) {
			m_stateStack->pop();
		}
		else {
			m_stateStack->pop();
			m_stateStack->push<TitleScreenState>(m_port);
		}
	});

	updateWidgetPosition();
}

void ConnectionErrorState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);
	}
}

void ConnectionErrorState::updateWidgetPosition() {
	m_background.setPosRect(0, 0, u16(Config::screenWidth / m_background.getScale().x), u16(Config::screenHeight / m_background.getScale().y));
	m_background.setClipRect(0, 0, u16(Config::screenWidth / m_background.getScale().x), u16(Config::screenHeight / m_background.getScale().y));

	m_filter.setSize(Config::screenWidth, Config::screenHeight);

	m_text.setPosition(float(Config::screenWidth  / 2 - m_text.getSize().x * Config::guiScale * 1.5 / 2),
	                   float(Config::screenHeight / 2 - m_text.getSize().y * Config::guiScale * 1.5 / 2));

	m_menuWidget.setPosition(
		Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2,
		Config::screenHeight - 110.f * Config::guiScale
	);
}

void ConnectionErrorState::draw(RenderTarget &target, RenderStates states) const {
	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_background, states);
		target.draw(m_filter, states);

		target.draw(m_text, states);

		target.draw(m_menuWidget, states);
	}
}
