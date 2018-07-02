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
#include <algorithm>

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "SettingsMenuState.hpp"
#include "World.hpp"

SettingsMenuState::SettingsMenuState(ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_background.setColor(Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) { m_stateStack->pop(); });

	m_menuWidget.addButton(0, 0, "Render distance: " + std::to_string(World::renderDistance), [] (TextButton &button) {
		World::renderDistance = std::max(4, (World::renderDistance + 2) % 16);
		button.setText("Render distance: " + std::to_string(World::renderDistance));
	});

	m_menuWidget.addButton(0, 1, "GUI Scale: " + std::to_string(GUI_SCALE), [] (TextButton &) {
		// GUI_SCALE = std::max(1, GUI_SCALE % 5);
		// button.setText("GUI Scale: " + std::to_string(GUI_SCALE));
	}).setEnabled(false);

	m_menuWidget.addButton(0, 2, "Fullscreen: OFF", [] (TextButton &) {}).setEnabled(false);
	m_menuWidget.addButton(0, 3, "Use VSync: OFF", [] (TextButton &) {}).setEnabled(false);
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
	states.vertexAttributes = VertexAttribute::Only2d;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}


