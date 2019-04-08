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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Debug.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/graphics/Color.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "FurnaceWidget.hpp"
#include "InventoryWidget.hpp"
#include "LuaGUIState.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "TextButton.hpp"

LuaGUIState::LuaGUIState(ClientCommandHandler &client, ClientPlayer &player, ClientWorld &world, sf::Packet &packet, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client)
{
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_background.setColor(gk::Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_mainWidget.setScale(GUI_SCALE, GUI_SCALE);

	while (!packet.endOfPacket())
		loadGUI(player, world, packet);
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

	prepareDraw(target, states);

	states.transform *= m_mainWidget.getTransform();
	states.viewMatrix = gk::Transform::Identity;

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

void LuaGUIState::loadGUI(ClientPlayer &player, ClientWorld &world, sf::Packet &packet) {
	u8 type;
	std::string name;
	float x, y;
	packet >> type >> name >> x >> y;
	if (type == 0) {
		std::string texture;
		gk::FloatRect clipRect;
		packet >> texture >> clipRect.x >> clipRect.y >> clipRect.width >> clipRect.height;

		auto *image = new gk::Image(texture);
		image->setPosition(x, y);
		image->setClipRect(clipRect.x, clipRect.y, clipRect.width, clipRect.height);
		m_drawables.emplace_back(image);
	}
	else if (type == 1) {
		std::string text;
		packet >> text;

		auto *button = new TextButton(&m_mainWidget);
		button->setPosition(x, y);
		// button->setCallback(it.on_click);
		button->setText(text);
		m_widgets.emplace_back(button);
	}
	else if (type == 2) {
		std::string playerName, inventory;
		float width, height;
		u16 offset, count;
		packet >> playerName >> inventory >> width >> height >> offset >> count;

		m_inventoryWidgets.emplace_back(m_client, &m_mainWidget);

		auto &inventoryWidget = m_inventoryWidgets.back();
		inventoryWidget.setPosition(x, y);
		inventoryWidget.init(player.inventory(), offset, count);
	}
	else if (type == 3) {
		gk::Vector3i block;
		u16 offset, count;
		packet >> block.x >> block.y >> block.z >> offset >> count;
		BlockData *data = world.getBlockData(block.x, block.y, block.z);
		if (data) {
			m_craftingWidgets.emplace_back(m_client, data->inventory, &m_mainWidget);

			auto &craftingWidget = m_craftingWidgets.back();
			craftingWidget.setPosition(x, y);
		}
		else {
			DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
		}
	}
	else if (type == 4) {
		gk::Vector3i block;
		packet >> block.x >> block.y >> block.z;
		BlockData *data = world.getBlockData(block.x, block.y, block.z);
		if (data) {
			auto *furnaceWidget = new FurnaceWidget(m_client, m_mouseItemWidget, player.inventory(), *data, &m_mainWidget);
			furnaceWidget->setPosition(x, y);
			m_widgets.emplace_back(furnaceWidget);
		}
		else {
			DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
		}
	}
}

