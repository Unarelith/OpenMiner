/*
 * =====================================================================================
 *
 *       Filename:  Crosshair.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 18:10:10
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Crosshair.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"

Crosshair::Crosshair() {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	float xFactor = SCREEN_WIDTH * SCREEN_HEIGHT / 100;
	float yFactor = SCREEN_HEIGHT * SCREEN_WIDTH / 100;

	m_hShape.setSize(0.002 * xFactor, 0.0002 * yFactor);
	m_vShape1.setSize(0.0002 * xFactor, 0.001 * yFactor - m_hShape.height() / 2);
	m_vShape2.setSize(0.0002 * xFactor, 0.001 * yFactor - m_hShape.height() / 2);

	m_hShape.setColor(Color{200, 200, 200, 180});
	m_vShape1.setColor(Color{200, 200, 200, 180});
	m_vShape2.setColor(Color{200, 200, 200, 180});

	m_hShape.setPosition(SCREEN_WIDTH / 2 - m_hShape.width() / 2, SCREEN_HEIGHT / 2 - m_hShape.height() / 2, 0);

	m_vShape1.setPosition(SCREEN_WIDTH / 2 - m_vShape1.width() / 2, SCREEN_HEIGHT / 2 - m_hShape.height() / 2 - m_vShape1.height(), 0);
	m_vShape2.setPosition(SCREEN_WIDTH / 2 - m_vShape2.width() / 2, SCREEN_HEIGHT / 2 + m_hShape.height() / 2, 0);
}

void Crosshair::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;

	// Shader::bind(&m_shader);
	// m_shader.setUniform("u_renderType", -1);
	// Shader::bind(nullptr);

	target.draw(m_hShape, states);
	target.draw(m_vShape1, states);
	target.draw(m_vShape2, states);
}

