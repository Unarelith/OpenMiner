/*
 * =====================================================================================
 *
 *       Filename:  LuaGUIState.cpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 00:48:31
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/graphics/Color.hpp>

#include "Config.hpp"
#include "InventoryWidget.hpp"
#include "LuaGUIState.hpp"
#include "Player.hpp"
#include "TextButton.hpp"
#include "World.hpp"

LuaGUIState::LuaGUIState(LuaGUI &gui, gk::ApplicationState *parent) : gk::ApplicationState(parent) {
	// FIXME: Duplicated with HUD
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_orthoMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_background.setColor(gk::Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_mainWidget.setScale(GUI_SCALE, GUI_SCALE);

	loadGUI(gui);
}

void LuaGUIState::onEvent(const SDL_Event &event) {
	for (auto &it : m_widgets)
		it->onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_stateStack->pop();
	}
}

void LuaGUIState::update() {
	if (m_parent)
		m_parent->update();

	for (auto &it : m_widgets)
		it->update();
}

void LuaGUIState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	states.transform *= m_mainWidget.getTransform();
	states.shader = &m_shader;
	states.projectionMatrix = m_orthoMatrix;
	states.viewMatrix = gk::Transform::Identity;
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	target.draw(m_background, states);

	for (auto &it : m_widgets)
		target.draw(*it, states);
}

void LuaGUIState::loadGUI(LuaGUI &gui) {
	for (auto &it : gui.buttons) {
		auto *button = new TextButton(&m_mainWidget);
		button->setPosition(it.x, it.y);
		button->setCallback(it.on_click);
		button->setText(it.text);
		m_widgets.emplace_back(button);
	}

	for (auto &it : gui.inventoryLists) {
		auto *inventoryWidget = new InventoryWidget(&m_mainWidget);
		inventoryWidget->setPosition(it.x, it.y);
		inventoryWidget->init(World::getInstance().getPlayer()->inventory(), it.offset, it.size);
		m_widgets.emplace_back(inventoryWidget);
	}
}

