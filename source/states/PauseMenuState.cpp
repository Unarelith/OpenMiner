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
#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "PauseMenuState.hpp"
#include "SettingsMenuState.hpp"

PauseMenuState::PauseMenuState(ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	Mouse::setCursorGrabbed(false);
	Mouse::setCursorVisible(true);
	Mouse::resetToWindowCenter();

	m_background.setColor(Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_menuWidget.addButton(0, 0, "Back to Game", [this] (TextButton &) { Mouse::setCursorGrabbed(true); Mouse::setCursorVisible(false); m_stateStack->pop(); });
	m_menuWidget.addButton(0, 1, "Options...", [this] (TextButton &) { m_stateStack->push<SettingsMenuState>(m_parent); });
	m_menuWidget.addButton(0, 2, "Exit", [this] (TextButton &) { while(!m_stateStack->empty()) m_stateStack->pop(); });
}

void PauseMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		Mouse::setCursorGrabbed(true);
		Mouse::setCursorVisible(false);

		m_stateStack->pop();
	}
}

void PauseMenuState::update() {
}

void PauseMenuState::draw(RenderTarget &target, RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	applyTransform(states);

	states.shader = &m_shader;
	states.vertexAttributes = VertexAttribute::Only2d;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}

