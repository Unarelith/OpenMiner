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
#include "Config.hpp"
#include "InterfaceState.hpp"

InterfaceState::InterfaceState(gk::ApplicationState *parent) : gk::ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	// m_view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// m_view.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void InterfaceState::prepareDraw(gk::RenderTarget &target, gk::RenderStates &states) const {
	states.transform *= getTransform();
	states.shader = &m_shader;
	// states.vertexAttributes = gk::VertexAttribute::Only2d;

	states.projectionMatrix = m_projectionMatrix;

	// target.setView(m_view);
}

