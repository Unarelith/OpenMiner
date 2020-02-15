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
#include <gk/resource/ResourceHandler.hpp>

#include "Config.hpp"
#include "GameState.hpp"
#include "ServerLoadingState.hpp"

ServerLoadingState::ServerLoadingState(GameState &game) : m_game(game) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_text.setFont(gk::ResourceHandler::getInstance().get<gk::Font>("font-default"));
	m_text.setCharacterSize(8 * 6);
	m_text.setString("Loading world...");
	m_text.setColor(gk::Color::White);
	m_text.setPosition(Config::screenWidth / 2.0f - m_text.getLocalBounds().width / 2.0f, 200);

	m_textShadow.setFont(gk::ResourceHandler::getInstance().get<gk::Font>("font-default"));
	m_textShadow.setCharacterSize(8 * 6);
	m_textShadow.setString(m_text.string());
	m_textShadow.setColor(gk::Color{70, 70, 70, 255});
	m_textShadow.setPosition(m_text.getPosition().x + 6, m_text.getPosition().y + 6);

	gk::Mouse::setCursorVisible(true);
	gk::Mouse::setCursorGrabbed(false);
}

void ServerLoadingState::update() {
	m_game.client().update();

	if (m_isWorldSent) {
		m_stateStack->pop();

		gk::Mouse::setCursorVisible(false);
		gk::Mouse::setCursorGrabbed(true);
	}
}

void ServerLoadingState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.shader = &m_shader;
	target.setView(target.getDefaultView());
	target.draw(m_textShadow, states);
	target.draw(m_text, states);
}

