/*
 * =====================================================================================
 *
 *       Filename:  InventoryState.cpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:13:50
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ApplicationStateStack.hpp"
#include "InventoryState.hpp"
#include "Mouse.hpp"

InventoryState::InventoryState(ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	Mouse::setCursorGrabbed(false);
	Mouse::setCursorVisible(true);
	Mouse::resetToWindowCenter();

	m_background.setColor(Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void InventoryState::onEvent(const SDL_Event &event) {
	// if (m_parent)
	// 	m_parent->onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		Mouse::setCursorGrabbed(true);
		Mouse::setCursorVisible(false);

		m_stateStack->pop();
	}

	if (m_widget)
		m_widget->onEvent(event);
}

void InventoryState::update() {
	if (m_parent)
		m_parent->update();

	if (m_widget)
		m_widget->update();
}

void InventoryState::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	if (m_parent)
		target.draw(*m_parent);

	states.shader = &m_shader;

	target.draw(m_background, states);

	if (m_widget)
		target.draw(*m_widget, states);
}

