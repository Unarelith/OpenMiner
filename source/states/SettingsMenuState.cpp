/*
 * =====================================================================================
 *
 *       Filename:  SettingsMenuState.cpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:11:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "SettingsMenuState.hpp"

SettingsMenuState::SettingsMenuState(ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_background.setColor(Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_menuWidget.addButton(0, 7, "Back to menu", [this] { m_stateStack->pop(); });

	m_menuWidget.addButton(0, 0, "VSync: OFF", [] {}).setEnabled(false);
	m_menuWidget.addButton(0, 1, "GUI scale: " + std::to_string(GUI_SCALE), [] {}).setEnabled(false);
	m_menuWidget.addButton(0, 2, "Fullscreen: OFF", [] {}).setEnabled(false);
	m_menuWidget.addButton(0, 3, "Render distance: 0", [] {}).setEnabled(false);
}

void SettingsMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		m_stateStack->pop();
	}
}

void SettingsMenuState::update() {
}

void SettingsMenuState::draw(RenderTarget &target, RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	applyTransform(states);

	states.shader = &m_shader;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}


