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
#include <gk/core/Debug.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/graphics/Color.hpp>

#include "Config.hpp"
#include "FurnaceWidget.hpp"
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
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_stateStack->pop();
	}

	for (auto &it : m_widgets)
		it->onEvent(event);

	for (auto &it : m_inventoryWidgets)
		it.onMouseEvent(event, m_mouseItemWidget, false);

	for (auto &it : m_craftingWidgets)
		it.onMouseEvent(event, m_mouseItemWidget);

	m_mouseItemWidget.onEvent(event);
}

void LuaGUIState::update() {
	if (m_parent)
		m_parent->update();

	for (auto &it : m_widgets)
		it->update();

	for (auto &it : m_craftingWidgets) {
		it.update();
	}

	const ItemWidget *currentItemWidget = nullptr;
	for (auto &it : m_inventoryWidgets) {
		if (!currentItemWidget)
			currentItemWidget = it.currentItemWidget();
	}
	for (auto &it : m_craftingWidgets) {
		if (!currentItemWidget)
			currentItemWidget = it.currentItemWidget();
	}

	m_mouseItemWidget.update(currentItemWidget);
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

	for (auto &it : m_drawables)
		target.draw(*it, states);

	for (auto &it : m_widgets)
		target.draw(*it, states);

	for (auto &it : m_inventoryWidgets)
		target.draw(it, states);

	for (auto &it : m_craftingWidgets)
		target.draw(it, states);

	target.draw(m_mouseItemWidget, states);
}

void LuaGUIState::loadGUI(LuaGUI &gui) {
	for (auto &it : gui.imageList) {
		auto *image = new gk::Image(it.texture);
		image->setPosition(it.x, it.y);
		image->setClipRect(it.clipRect.x, it.clipRect.y, it.clipRect.width, it.clipRect.height);
		m_drawables.emplace_back(image);
	}

	for (auto &it : gui.textButtonList) {
		auto *button = new TextButton(&m_mainWidget);
		button->setPosition(it.x, it.y);
		button->setCallback(it.on_click);
		button->setText(it.text);
		m_widgets.emplace_back(button);
	}

	for (auto &it : gui.inventoryWidgetList) {
		auto &inventoryWidget = m_inventoryWidgets.emplace_back(&m_mainWidget);
		inventoryWidget.setPosition(it.x, it.y);
		inventoryWidget.init(World::getInstance().getPlayer()->inventory(), it.offset, it.count);
	}

	for (auto &it : gui.craftingWidgetList) {
		BlockData *data = World::getInstance().getBlockData(it.block.x, it.block.y, it.block.z);
		if (data) {
			auto &craftingWidget = m_craftingWidgets.emplace_back(data->inventory, &m_mainWidget);
			craftingWidget.setPosition(it.x, it.y);
		}
		else {
			DEBUG("ERROR: No inventory found at", it.block.x, it.block.y, it.block.z);
		}
	}

	for (auto &it : gui.furnaceWidgetList) {
		BlockData *data = World::getInstance().getBlockData(it.block.x, it.block.y, it.block.z);
		if (data) {
			auto *furnaceWidget = new FurnaceWidget(m_mouseItemWidget, World::getInstance().getPlayer()->inventory(), *data, &m_mainWidget);
			furnaceWidget->setPosition(it.x, it.y);
			m_widgets.emplace_back(furnaceWidget);
		}
		else {
			DEBUG("ERROR: No inventory found at", it.block.x, it.block.y, it.block.z);
		}
	}
}

