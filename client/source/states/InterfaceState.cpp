/*
 * =====================================================================================
 *
 *       Filename:  InterfaceState.cpp
 *
 *    Description:
 *
 *        Created:  08/04/2019 18:26:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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

