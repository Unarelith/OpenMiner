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
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>

#include "Config.hpp"
#include "ChatState.hpp"

ChatState::ChatState(gk::ApplicationState *parent) : InterfaceState(parent) {
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_drawBackground = false;

	m_textInput.setScale(Config::guiScale, Config::guiScale);
	m_textInput.setBackgroundColor(gk::Color{0, 0, 0, 127});
	m_textInput.setBackgroundSize(200, 10);
	m_textInput.setPadding(1, 1);
}

void ChatState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	m_textInput.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_stateStack->pop();
	}
}

void ChatState::update() {
}

void ChatState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_textInput, states);
	}
}

