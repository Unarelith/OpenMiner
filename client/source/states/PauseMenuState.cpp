/*
 * =====================================================================================
 *
 *       Filename:  PauseMenuState.cpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:15:27
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>

#include "Config.hpp"
#include "PauseMenuState.hpp"
#include "SettingsMenuState.hpp"

PauseMenuState::PauseMenuState(gk::ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_background.setColor(gk::Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_menuWidget.addButton("Back to Game", [this] (TextButton &) { gk::Mouse::setCursorGrabbed(true); gk::Mouse::setCursorVisible(false); m_stateStack->pop(); });
	m_menuWidget.addButton("Options...", [this] (TextButton &) { m_stateStack->push<SettingsMenuState>(m_parent); });
	m_menuWidget.addButton("Exit", [this] (TextButton &) { while(!m_stateStack->empty()) m_stateStack->pop(); });
}

void PauseMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_stateStack->pop();
	}
}

void PauseMenuState::update() {
}

void PauseMenuState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	states.transform *= getTransform();

	states.projectionMatrix = m_projectionMatrix;

	states.shader = &m_shader;
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}

