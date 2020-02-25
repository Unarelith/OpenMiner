/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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
#include "InventoryWidget.hpp"
#include "LuaGUIState.hpp"
#include "LuaWidget.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "ProgressBarWidget.hpp"
#include "TextButton.hpp"

LuaGUIState::LuaGUIState(ClientCommandHandler &client, ClientPlayer &player, ClientWorld &world, sf::Packet &packet, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client), m_player(player), m_world(world)
{
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_mainWidget.setScale(Config::guiScale, Config::guiScale);

	while (!packet.endOfPacket())
		loadGUI(packet);
}

void LuaGUIState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

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

	for (auto &it : m_inventoryWidgets) {
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

	m_mouseItemWidget.updateCurrentItem(currentItemWidget);
}

void LuaGUIState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	prepareDraw(target, states);

	states.transform *= m_mainWidget.getTransform();
	states.viewMatrix = gk::Transform::Identity;

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

void LuaGUIState::loadGUI(sf::Packet &packet) {
	u8 type;
	std::string name;
	s32 x, y;
	packet >> type >> name >> x >> y;

	if (type == LuaWidget::Image)
		loadImage(name, x, y, packet);
	else if (type == LuaWidget::TextButton)
		loadTextButton(name, x, y, packet);
	else if (type == LuaWidget::InventoryWidget)
		loadInventoryWidget(name, x, y, packet);
	else if (type == LuaWidget::ProgressBarWidget)
		loadProgressBarWidget(name, x, y, packet);
	else if (type == LuaWidget::CraftingWidget)
		loadCraftingWidget(name, x, y, packet);
}

void LuaGUIState::loadImage(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string texture;
	gk::FloatRect clipRect;
	packet >> texture >> clipRect.x >> clipRect.y >> clipRect.sizeX >> clipRect.sizeY;

	auto *image = new gk::Image(texture);
	image->setPosition(x, y);
	image->setClipRect(clipRect.x, clipRect.y, clipRect.sizeX, clipRect.sizeY);
	m_drawables.emplace_back(image);
}

void LuaGUIState::loadTextButton(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string text;
	packet >> text;

	auto *button = new TextButton(&m_mainWidget);
	button->setPosition(x, y);
	// button->setCallback(it.on_click);
	button->setText(text);
	m_widgets.emplace_back(button);
}

void LuaGUIState::loadInventoryWidget(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string inventory, playerName, inventory_name;
	gk::Vector3i block;
	float width, height;
	u16 offset, count;
	packet >> inventory >> playerName >> inventory_name
		>> block.x >> block.y >> block.z
		>> width >> height >> offset >> count;

	Inventory *widgetInventory = nullptr;
	if (inventory == "player") {
		widgetInventory = &m_player.inventory();
	}
	else if (inventory == "block") {
		BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
		if (!data) {
			DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
			return;
		}

		widgetInventory = &data->inventory;
	}

	if (widgetInventory) {
		m_inventoryWidgets.emplace_back(m_client, &m_mainWidget);

		auto &inventoryWidget = m_inventoryWidgets.back();
		inventoryWidget.setPosition(x, y);
		inventoryWidget.init(*widgetInventory, offset, count);
	}
	else {
		DEBUG("ERROR: Widget inventory is invalid");
	}
}

void LuaGUIState::loadCraftingWidget(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string inventory;
	gk::Vector3i block;
	u16 offset, size;
	s32 resultX, resultY;
	packet >> inventory >> block.x >> block.y >> block.z >> offset >> size >> resultX >> resultY;

	Inventory *craftingInventory = nullptr;
	if (inventory == "block") {
		BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
		if (!data) {
			DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
			return;
		}

		craftingInventory = &data->inventory;
	}
	else if (inventory == "temp") {
		craftingInventory = &m_inventory;
		m_inventory.resize(size, size);
	}

	if (craftingInventory) {
		m_craftingWidgets.emplace_back(m_client, *craftingInventory, &m_mainWidget);

		auto &craftingWidget = m_craftingWidgets.back();
		craftingWidget.init(offset, size);
		craftingWidget.craftingInventoryWidget().setPosition(x, y);
		craftingWidget.craftingResultInventoryWidget().setPosition(resultX, resultY);
	}
	else {
		DEBUG("ERROR: Crafting inventory is invalid");
	}
}

void LuaGUIState::loadProgressBarWidget(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	u8 type;
	gk::Vector3i block;
	std::string meta, maxMeta;
	u32 maxValue;
	std::string texture;
	gk::FloatRect clipRect;
	packet >> type >> block.x >> block.y >> block.z >> meta >> maxMeta >> maxValue >> texture
		>> clipRect.x >> clipRect.y >> clipRect.sizeX >> clipRect.sizeY;

	BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
	if (!data) {
		DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
		return;
	}

	ProgressBarWidget *widget = new ProgressBarWidget(texture, *data, ProgressBarType(type));
	if (!maxMeta.empty())
		widget->init(clipRect, gk::Vector2i{x, y}, meta, maxMeta);
	else
		widget->init(clipRect, gk::Vector2i{x, y}, meta, maxValue);

	m_widgets.emplace_back(widget);
}

