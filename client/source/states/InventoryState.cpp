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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>

#include "InventoryState.hpp"

InventoryState::InventoryState(gk::ApplicationState *parent) : InterfaceState(parent) {
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();
}

void InventoryState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (m_widget) {
			m_widget->setPosition(SCREEN_WIDTH  / 2.0 - m_widget->getGlobalBounds().width  / 2.0,
			                      SCREEN_HEIGHT / 2.0 - m_widget->getGlobalBounds().height / 2.0, 0);
		}
	}

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

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

void InventoryState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	prepareDraw(target, states);

	if (m_widget)
		target.draw(*m_widget, states);
}

