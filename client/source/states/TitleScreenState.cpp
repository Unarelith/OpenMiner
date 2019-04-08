/*
 * =====================================================================================
 *
 *       Filename:  TitleScreenState.cpp
 *
 *    Description:
 *
 *        Created:  08/04/2019 18:00:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "GameState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"

TitleScreenState::TitleScreenState() {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	m_menuWidget.addButton("Play", [this] (TextButton &) {
		m_stateStack->push<GameState>();
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>();
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		m_stateStack->pop();
	});
}

void TitleScreenState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);
}

void TitleScreenState::update() {
}

void TitleScreenState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	states.projectionMatrix = m_projectionMatrix;

	states.shader = &m_shader;
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	target.draw(m_menuWidget, states);
}

