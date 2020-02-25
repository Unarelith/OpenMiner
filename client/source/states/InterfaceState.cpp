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

#include <gk/core/ApplicationStateStack.hpp>

#include "Config.hpp"
#include "InterfaceState.hpp"

InterfaceState::InterfaceState(gk::ApplicationState *parent) : gk::ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_background.setFillColor(gk::Color{0, 0, 0, 127});

	setup();
}

void InterfaceState::setup() {
	m_projectionMatrix = glm::ortho(0.0f, (float)Config::screenWidth, (float)Config::screenHeight, 0.0f);

	m_background.setSize(Config::screenWidth, Config::screenHeight);

	// m_view.setSize(Config::screenWidth, Config::screenHeight);
	// m_view.setCenter(Config::screenWidth / 2.0f, Config::screenHeight / 2.0f);
}

void InterfaceState::onEvent(const SDL_Event &event) {
	if (m_parent) {
		m_parent->onEvent(event);
	}
	else if (event.type == SDL_QUIT) {
		m_stateStack->clear();
	}

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (!m_parent) {
			Config::screenWidth = event.window.data1;
			Config::screenHeight = event.window.data2;
		}

		setup();
	}
}

void InterfaceState::update() {
	if (m_parent)
		m_parent->update();
}

void InterfaceState::prepareDraw(gk::RenderTarget &target, gk::RenderStates &states) const {
	states.transform *= getTransform();
	states.shader = &m_shader;
	// states.vertexAttributes = gk::VertexAttribute::Only2d;

	states.projectionMatrix = m_projectionMatrix;

	// target.setView(m_view);

	if (m_parent && m_drawBackground)
		target.draw(m_background, states);
}

