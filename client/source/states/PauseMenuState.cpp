/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>

#include "Client.hpp"
#include "Config.hpp"
#include "PauseMenuState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"

PauseMenuState::PauseMenuState(Client &client, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client)
{
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	m_menuWidget.addButton("Back to Game", [this] (TextButton &) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);

		m_stateStack->pop();
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>(this);
	});

	m_menuWidget.addButton("Title Screen", [this] (TextButton &) {
		// m_client.disconnect();

		m_stateStack->clear();
		m_stateStack->push<TitleScreenState>();
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		// m_client.disconnect();

		while(!m_stateStack->empty())
			m_stateStack->pop();
	});
}

void PauseMenuState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (&m_stateStack->top() != this)
			m_menuWidget.onEvent(event);
	}

	if (&m_stateStack->top() == this) {
		m_menuWidget.onEvent(event);

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
			gk::Mouse::resetToWindowCenter();

			m_stateStack->pop();
		}
	}
}

void PauseMenuState::update() {
}

void PauseMenuState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_menuWidget, states);
	}
}

