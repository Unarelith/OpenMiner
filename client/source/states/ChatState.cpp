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

#include "Chat.hpp"
#include "ChatState.hpp"
#include "ClientCommandHandler.hpp"
#include "Config.hpp"

ChatState::ChatState(ClientCommandHandler &clientCommandHandler, Chat &chat, bool addSlash, gk::ApplicationState *parent)
	: InterfaceState(parent), m_clientCommandHandler(clientCommandHandler), m_chat(chat)
{
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_drawBackground = false;

	m_textInput.setScale(Config::guiScale, Config::guiScale);
	m_textInput.setBackgroundColor(gk::Color{0, 0, 0, 127});
	m_textInput.setPadding(1, 1);

	updateTextInputGeometry();

	if (addSlash)
		m_textInput.setText("/");

	m_chat.setMessageVisibility(true);
}

void ChatState::updateTextInputGeometry() {
	m_textInput.setPosition(4, Config::screenHeight - 12 * Config::guiScale);
	m_textInput.setBackgroundSize(Config::screenWidth / Config::guiScale - 4, 10);
}

void ChatState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	m_textInput.onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		updateTextInputGeometry();
	}

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_chat.setMessageVisibility(false);

		m_stateStack->pop();
	}

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
		if (!m_textInput.text().empty())
			m_clientCommandHandler.sendChatMessage(m_textInput.text());

		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_chat.setMessageVisibility(false);

		m_stateStack->pop();
	}
}

void ChatState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	if (&m_stateStack->top() == this) {
		prepareDraw(target, states);

		target.draw(m_textInput, states);
	}
}

