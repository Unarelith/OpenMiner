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
#include <gk/graphics/Color.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "GameKey.hpp"
#include "InventoryWidget.hpp"
#include "LuaGUIState.hpp"
#include "LuaWidget.hpp"
#include "Mouse.hpp"
#include "Network.hpp"
#include "NetworkUtils.hpp"
#include "Player.hpp"
#include "ProgressBarWidget.hpp"
#include "Registry.hpp"
#include "ScrollBarWidget.hpp"
#include "TextButton.hpp"

bool LuaGUIState::isActive = false;

LuaGUIState::LuaGUIState(ClientCommandHandler &client, ClientPlayer &player, ClientWorld &world, sf::Packet &packet, gk::ApplicationState *parent)
	: InterfaceState(parent), m_client(client), m_player(player), m_world(world)
{
	Mouse::setCursorGrabbed(false);
	Mouse::setCursorVisible(true);
	Mouse::resetToWindowCenter();

	m_mainWidget.setScale(Config::guiScale, Config::guiScale);

	packet >> m_width >> m_height >> m_isCentered >> m_keyID;

	if (m_isCentered)
		centerMainWidget();

	while (!packet.endOfPacket())
		loadGUI(packet);

	isActive = true;
}

void LuaGUIState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && m_isCentered)
		centerMainWidget();

	if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_ESCAPE
	 || (m_keyID >= 0 && event.key.keysym.sym == Registry::getInstance().getKey(m_keyID).keycode()))) {
		bool ignoreExit = false;
		for (auto &it : m_textInputs) {
			if (it.second.hasFocus()) {
				ignoreExit = true;

				if (event.key.keysym.sym == SDLK_ESCAPE)
					it.second.setFocus(false);
			}
		}

		if (!ignoreExit) {
			Mouse::setCursorGrabbed(true);
			Mouse::setCursorVisible(false);
			Mouse::resetToWindowCenter();

			isActive = false;

			m_stateStack->pop();
		}
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
				gkWarning() << "Destination not found:" << shiftDestination;
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

	for (auto &it : m_textInputs)
		it.second.onEvent(event);
}

void LuaGUIState::update() {
	InterfaceState::update();

	for (auto &it : m_textInputInventories) {
		auto inv = m_inventoryWidgets.find(it.second);
		if (inv != m_inventoryWidgets.end()) {
			inv->second.applySearch(m_textInputs.at(it.first).string());
		}
		else
			gkError() << "Can't find linked inventory" << it.second << "for text input" << it.first;
	}

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

	m_mouseItemWidget.setCurrentInventoryWidget(m_currentInventoryWidget);
	m_mouseItemWidget.updateCurrentItem(currentItemWidget);
}

void LuaGUIState::draw(gk::RenderTarget &target, RenderStates states) const {
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

	for (auto &it : m_textInputs)
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
	else if (type == LuaWidget::TextInput)
		loadTextInput(name, x, y, packet);
	else if (type == LuaWidget::Inventory)
		loadInventory(name, packet);
}

void LuaGUIState::loadImage(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string textureFilename;
	gk::FloatRect clipRect;
	packet >> textureFilename >> clipRect.x >> clipRect.y >> clipRect.sizeX >> clipRect.sizeY;

	gk::Texture &texture = loadTexture(textureFilename);

	auto *image = new gk::Image(texture);
	image->setPosition((float)x, (float)y);
	image->setClipRect(clipRect.x, clipRect.y, (u16)clipRect.sizeX, (u16)clipRect.sizeY);
	m_drawables.emplace_back(image);
}

void LuaGUIState::loadTextButton(const std::string &, s32 x, s32 y, sf::Packet &packet) {
	std::string text;
	packet >> text;

	auto *button = new TextButton(&m_mainWidget);
	button->setPosition((float)x, (float)y);
	// button->setCallback(it.on_click);
	button->setText(text);
	m_widgets.emplace_back(button);
}

void LuaGUIState::loadInventoryWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet) {
	std::string inventory, shiftDestination, filter;
	u16 width, height;
	u16 offset, count;
	bool isReadOnly;
	packet >> width >> height >> shiftDestination >> offset >> count >> inventory
		>> filter >> isReadOnly;

	Inventory *widgetInventory = nullptr;
	if (inventory == "player") {
		std::string playerName, inventoryName;
		packet >> playerName >> inventoryName;

		widgetInventory = &m_player.inventory();
	}
	else if (inventory == "block") {
		gk::Vector3i block;
		packet >> block;

		BlockData *data = m_world.getBlockData(block.x, block.y, block.z);
		if (!data) {
			gkError() << "No inventory found at" << block.x << block.y << block.z;
			return;
		}

		widgetInventory = &data->inventory;
	}
	else if (inventory == "temp") {
		std::string inventoryName;
		packet >> inventoryName;

		if (inventoryName.empty()) {
			m_inventories.emplace("_temp", Inventory{width, height});
			widgetInventory = &m_inventories.at("_temp");
		}
		else {
			auto it = m_inventories.find(inventoryName);
			if (it == m_inventories.end())
				gkError() << "Unable to find inventory" << inventoryName << "for widget" << name;

			widgetInventory = &it->second;
		}
	}

	if (widgetInventory) {
		m_inventoryWidgets.emplace(name, InventoryWidget{m_client, isReadOnly, &m_mainWidget});

		auto &inventoryWidget = m_inventoryWidgets.at(name);
		inventoryWidget.setPosition((float)x, (float)y);
		inventoryWidget.init(*widgetInventory, offset, count);
		inventoryWidget.setShiftDestination(shiftDestination);
		inventoryWidget.setFilter(filter);
	}
	else {
		gkError() << "Inventory widget" << name << " is invalid";
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
			gkError() << "No inventory found at" << block.x << block.y << block.z;
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
		craftingWidget.craftingInventoryWidget().setPosition((float)x, (float)y);
		craftingWidget.craftingResultInventoryWidget().setPosition((float)resultX, (float)resultY);
	}
	else {
		gkError() << "Crafting inventory is invalid";
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
		gkError() << "No inventory found at" << block.x << block.y << block.z;
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
	scrollBarWidget->setPosition((float)x, (float)y);
	scrollBarWidget->init(texture, clipRect, minY, maxY, m_inventoryWidgets.at(widget));

	m_widgets.emplace_back(scrollBarWidget);
}

void LuaGUIState::loadTextInput(const std::string &name, s32 x, s32 y, sf::Packet &packet) {
	u16 width, height;
	std::string placeholder, inventory;
	gk::Color placeholderColor;
	packet >> width >> height >> placeholder >> placeholderColor >> inventory;

	TextInput textInput{&m_mainWidget};
	textInput.setPosition((float)x, (float)y);
	textInput.setFocus(false);
	textInput.setBackgroundSize(width, height);
	textInput.setPlaceholder(placeholder);
	textInput.setPlaceholderColor(placeholderColor);

	m_textInputs.emplace(name, std::move(textInput));
	m_textInputInventories.emplace(name, inventory);
}

void LuaGUIState::loadInventory(const std::string &name, sf::Packet &packet) {
	m_inventories.emplace(name, Inventory{});

	packet >> m_inventories.at(name);
}

gk::Texture &LuaGUIState::loadTexture(const std::string &textureFilename) {
	auto it = m_textures.find(textureFilename);
	if (it == m_textures.end()) {
		m_textures.emplace(textureFilename, gk::Texture{}).first->second.loadFromFile(textureFilename);
		return m_textures.at(textureFilename);
	}
	else {
		return it->second;
	}
}

void LuaGUIState::centerMainWidget() {
	float x = floorf(Config::screenWidth  / 2.0f - m_width  * Config::guiScale / 2.0f + 0.5f);
	float y = floorf(Config::screenHeight / 2.0f - m_height * Config::guiScale / 2.0f + 0.5f);
	m_mainWidget.setPosition(x, y);
}

