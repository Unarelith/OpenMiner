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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/EventHandler.hpp>

#include "ApplicationStateStack.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Events.hpp"
#include "GameConfig.hpp"
#include "Mouse.hpp"
#include "PauseMenuState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"
#include "WorldSavingState.hpp"

PauseMenuState::PauseMenuState(Client &client, DrawableState *parent)
	: InterfaceState(parent), m_client(client)
{
	Mouse::setCursorGrabbed(false);
	Mouse::setCursorVisible(true);
	Mouse::resetToWindowCenter();

	m_menuWidget.setScale(Config::guiScale, Config::guiScale, 1);

	m_menuWidget.addButton("Back to Game", [this] (TextButton &) {
		Mouse::setCursorGrabbed(true);
		Mouse::setCursorVisible(false);

		m_stateStack->pop();
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>(this);
	});

	m_menuWidget.addButton("Title Screen", [this] (TextButton &) {
		if (!m_client.isSingleplayer() || GameConfig::worldName.empty()) {
			m_client.disconnect();

			GameConfig::isGameRunning = false;

			m_stateStack->clear();
			m_stateStack->push<TitleScreenState>();
		}
		else
			m_stateStack->push<WorldSavingState>(m_client, false);
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		if (!m_client.isSingleplayer() || GameConfig::worldName.empty()) {
			m_client.disconnect();

			m_stateStack->clear();
		}
		else
			m_stateStack->push<WorldSavingState>(m_client, true);
	});

	updateWidgetPosition();
}

void PauseMenuState::init() {
	m_eventHandler->addListener<GuiScaleChangedEvent>(&PauseMenuState::onGuiScaleChanged, this);
}

void PauseMenuState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
	&& !m_stateStack->empty() && &m_stateStack->top() != this) {
		m_menuWidget.onEvent(event);
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			Mouse::setCursorGrabbed(true);
			Mouse::setCursorVisible(false);
			Mouse::resetToWindowCenter();

			m_stateStack->pop();
		}
	}
}

void PauseMenuState::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	m_menuWidget.setScale(event.guiScale, event.guiScale);

	m_menuWidget.onGuiScaleChanged(event);
}

void PauseMenuState::updateWidgetPosition() {
	m_menuWidget.setPosition(
		roundf(Config::screenWidth / 2.0f - m_menuWidget.getGlobalBounds().sizeX / 2.0f),
		roundf(Config::screenHeight / 2.0f - m_menuWidget.getGlobalBounds().sizeY / 2.0f)
	);
}

void PauseMenuState::draw(RenderTarget &target, RenderStates states) const {
	if (m_parent)
		target.draw(*(DrawableState *)m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_menuWidget, states);
	}
}

