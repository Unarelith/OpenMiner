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
#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/graphics/Color.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "GameKey.hpp"
#include "InventoryWidget.hpp"
#include "LuaGUIState.hpp"
#include "LuaWidget.hpp"
#include "Network.hpp"
#include "NetworkUtils.hpp"
#include "Player.hpp"
#include "ProgressBarWidget.hpp"
#include "ScrollBarWidget.hpp"
#include "TextButton.hpp"

LuaGUIState::LuaGUIState(ClientCommandHandler &client, ClientPlayer &player, ClientWorld &world, sf::Packet &packet, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client), m_player(player), m_world(world)
{
	gk::Mouse::setCursorGrabbed(false);
	gk::Mouse::setCursorVisible(true);
	gk::Mouse::resetToWindowCenter();

	m_mainWidget.setScale(Config::guiScale, Config::guiScale);

	packet >> m_width >> m_height >> m_isCentered;

	if (m_isCentered)
		centerMainWidget();

	while (!packet.endOfPacket())
		loadGUI(packet);
}

void LuaGUIState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && m_isCentered)
		centerMainWidget();

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		gk::Mouse::setCursorGrabbed(true);
		gk::Mouse::setCursorVisible(false);
		gk::Mouse::resetToWindowCenter();

		m_stateStack->pop();
	}

	for (auto &it : m_widgets)
		it->onEvent(event);

	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
	 && m_currentInventoryWidget && !m_currentInventoryWidget->shiftDestination().empty()
	 && m_mouseItemWidget.currentItemWidget() && gk::GamePad::isKeyPressed(GameKey::Shift)) {
		for (const std::string &shiftDestination : m_currentInventoryWidget->shiftDestination()) {
			AbstractInventoryWidget *dest = nullptr;

			auto it = m_craftingWidgets.find(shiftDestination);
			if (it != m_craftingWidgets.end())
				dest = &it->second;

			if (!dest) {
				auto it = m_inventoryWidgets.find(shiftDestination);
				if (it != m_inventoryWidgets.end())
					dest = &it->second;
			}

			if (!dest) {
				DEBUG("WARNING: Destination not found: '" + shiftDestination + "'");
				return;
			}

			if (m_currentInventoryWidget->sendItemStackToDest(m_mouseItemWidget.currentItemWidget(), dest))
				break;
		}
	}
	else {
		for (auto &it : m_inventoryWidgets)
			it.second.onEvent(event);

		for (auto &it : m_craftingWidgets)
			it.second.onEvent(event);

		m_mouseItemWidget.onEvent(event);
	}
}

void LuaGUIState::update() {
	InterfaceState::update();

	for (auto &it : m_widgets)
		it->update();

	for (auto &it : m_craftingWidgets) {
		it.second.update();
	}

	for (auto &it : m_inventoryWidgets) {
		it.second.update();
	}

	ItemWidget *currentItemWidget = nullptr;
	m_currentInventoryWidget = nullptr;
	for (auto &it : m_inventoryWidgets) {
		if (!currentItemWidget && ((currentItemWidget = it.second.currentItemWidget()))) {
			m_currentInventoryWidget = &it.second;
		}
	}
	for (auto &it : m_craftingWidgets) {
		if (!currentItemWidget && ((currentItemWidget = it.second.currentItemWidget()))) {
			m_currentInventoryWidget = it.second.currentInventoryWidget();
		}
	}

	m_mouseItemWidget.updateCurrentItem(currentItemWidget);
	m_mouseItemWidget.setCurrentInventoryWidget(m_currentInventoryWidget);
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
		target.draw(it.second, states);

	for (auto &it : m_craftingWidgets)
		target.draw(it.second, states);

	target.draw(m_mouseItemWidget, states);
}

void LuaGUIState::loadGUI(sf::Packet &packet) {
	u8 type;
	std::string name;
	packet >> type >> name;

	s32 x, y;
	if (type != LuaWidget::Inventory)
		packet >> x >> y;

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
	else if (type == LuaWidget::ScrollBarWidget)
		loadScrollBarWidget(name, x, y, packet);
	else if (type == LuaWidget::Inventory)
		loadInventory(name, packet);
}

void LuaGUIState::loadImage(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string textureFilename;
	gk::FloatRect clipRect;
	packet >> textureFilename >> clipRect.x >> clipRect.y >> clipRect.sizeX >> clipRect.sizeY;

	gk::Texture &texture = loadTexture(textureFilename);

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

void LuaGUIState::loadInventoryWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet) {
	std::string inventory, playerName, inventory_name, shiftDestination;
	gk::Vector3i block;
	u16 width, height;
	u16 offset, count;
	packet >> inventory >> playerName >> inventory_name
		>> block.x >> block.y >> block.z >> shiftDestination
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
	else if (inventory == "temp") {
		if (inventory_name.empty()) {
			m_inventories.emplace("_temp", Inventory{width, height});
			widgetInventory = &m_inventories.at("_temp");
		}
		else {
			auto it = m_inventories.find(inventory_name);
			if (it == m_inventories.end())
				DEBUG("ERROR: Unable to find inventory '" + inventory_name + "' for widget '" + name + "'");

			widgetInventory = &it->second;
		}
	}

	if (widgetInventory) {
		m_inventoryWidgets.emplace(name, InventoryWidget{m_client, false, &m_mainWidget});

		auto &inventoryWidget = m_inventoryWidgets.at(name);
		inventoryWidget.setPosition(x, y);
		inventoryWidget.init(*widgetInventory, offset, count);
		inventoryWidget.setShiftDestination(shiftDestination);
	}
	else {
		DEBUG("ERROR: Inventory widget '" + name + "' is invalid");
	}
}

void LuaGUIState::loadCraftingWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet) {
	std::string inventory, shiftDestination;
	s32 resultX, resultY;
	packet >> resultX >> resultY >> shiftDestination >> inventory;

	u16 offset = 0, size;
	Inventory *craftingInventory = nullptr;
	if (inventory == "block") {
		gk::Vector3i block;
		packet >> block >> offset >> size;

		BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
		if (!data) {
			DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
		}
		else {
			craftingInventory = &data->inventory;
		}
	}
	else if (inventory == "temp") {
		packet >> size;

		m_inventories.emplace("_temp", Inventory{size, size});
		craftingInventory = &m_inventories.at("_temp");
	}

	if (craftingInventory) {
		m_craftingWidgets.emplace(name, CraftingWidget{m_client, *craftingInventory, &m_mainWidget});

		auto &craftingWidget = m_craftingWidgets.at(name);
		craftingWidget.setShiftDestination(shiftDestination);
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
	std::string textureFilename;
	gk::FloatRect clipRect;
	packet >> type >> block.x >> block.y >> block.z >> meta >> maxMeta >> maxValue >> textureFilename
		>> clipRect.x >> clipRect.y >> clipRect.sizeX >> clipRect.sizeY;

	BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
	if (!data) {
		DEBUG("ERROR: No inventory found at", block.x, block.y, block.z);
		return;
	}

	gk::Texture &texture = loadTexture(textureFilename);

	ProgressBarWidget *widget = new ProgressBarWidget(texture, *data, ProgressBarType(type));
	if (!maxMeta.empty())
		widget->init(clipRect, gk::Vector2i{x, y}, meta, maxMeta);
	else
		widget->init(clipRect, gk::Vector2i{x, y}, meta, maxValue);

	m_widgets.emplace_back(widget);
}

void LuaGUIState::loadScrollBarWidget(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string textureFilename;
	gk::FloatRect clipRect;
	u16 minY, maxY;
	std::string widget;
	packet >> textureFilename >> clipRect >> minY >> maxY >> widget;

	gk::Texture &texture = loadTexture(textureFilename);

	ScrollBarWidget *scrollBarWidget = new ScrollBarWidget(&m_mainWidget);
	scrollBarWidget->setPosition(x, y);
	scrollBarWidget->init(texture, clipRect, minY, maxY, m_inventoryWidgets.at(widget));

	m_widgets.emplace_back(scrollBarWidget);
}

void LuaGUIState::loadInventory(const std::string &name, sf::Packet &packet) {
	m_inventories.emplace(name, Inventory{});

	packet >> m_inventories.at(name);
}

gk::Texture &LuaGUIState::loadTexture(const std::string &textureFilename) {
	auto it = m_textures.find(textureFilename);
	if (it == m_textures.end()) {
		m_textures.emplace(textureFilename, gk::Texture{textureFilename});
		return m_textures.at(textureFilename);
	}
	else {
		return it->second;
	}
}

void LuaGUIState::centerMainWidget() {
	int x = floor(Config::screenWidth  / 2.0f - m_width  * Config::guiScale / 2.0f + 0.5f);
	int y = floor(Config::screenHeight / 2.0f - m_height * Config::guiScale / 2.0f + 0.5f);
	m_mainWidget.setPosition(x, y);
}

