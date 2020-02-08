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
	m_projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// m_view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// m_view.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void InterfaceState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (m_parent)
			m_parent->onEvent(event);
		else {
			SCREEN_WIDTH = event.window.data1;
			SCREEN_HEIGHT = event.window.data2;
		}

		setup();
	}
}

void InterfaceState::prepareDraw(gk::RenderTarget &target, gk::RenderStates &states) const {
	states.transform *= getTransform();
	states.shader = &m_shader;
	// states.vertexAttributes = gk::VertexAttribute::Only2d;

	states.projectionMatrix = m_projectionMatrix;

	// target.setView(m_view);

	if (m_parent)
		target.draw(m_background, states);
}

