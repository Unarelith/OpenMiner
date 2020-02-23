/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>

#include "LuaGUI.hpp"
#include "LuaWidget.hpp"
#include "Network.hpp"

void LuaGUI::addImage(const sol::table &table) {
	// FIXME: Duplicated below
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	gk::FloatRect clipRect;
	std::string texture = table["texture"].get<std::string>();
	sol::optional<sol::table> clipRectTable = table["clip"];
	if (clipRectTable != sol::nullopt) {
		clipRect.x = clipRectTable.value()["x"];
		clipRect.y = clipRectTable.value()["y"];
		clipRect.sizeX = clipRectTable.value()["width"];
		clipRect.sizeY = clipRectTable.value()["height"];
	}

	m_data.imageList.emplace_back(); // LuaWidgetDef::Image{{name, x, y}, texture, clipRect});

	LuaWidgetDef::Image &image = m_data.imageList.back();
	image.name = name;
	image.x = x;
	image.y = y;
	image.texture = texture;
	image.clipRect = clipRect;
}

void LuaGUI::addTextButton(const sol::table &table) {
	// FIXME: Duplicated below
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	std::string text = table["text"].get<std::string>();
	sol::function on_click = table["on_click"].get<sol::function>();
	m_data.textButtonList.emplace_back(); // LuaWidgetDef::TextButton{{name, x, y}, text, on_click});

	LuaWidgetDef::TextButton &button = m_data.textButtonList.back();
	button.name = name;
	button.x = x;
	button.y = y;
	button.text = text;
	button.on_click = on_click;
}

void LuaGUI::addInventoryWidget(const sol::table &table) {
	// FIXME: Duplicated above
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	gk::Vector3i block;
	std::string player, inventory_name;

	std::string inventory = table["inventory"].get<std::string>();
	if (inventory == "player") {
		player = table["player"].get<std::string>();
		inventory_name = table["inventory_name"].get<std::string>();
	}
	else if (inventory == "block") {
		sol::optional<sol::table> blockTable = table["block"];
		if (blockTable != sol::nullopt) {
			block.x = blockTable.value()["x"];
			block.y = blockTable.value()["y"];
			block.z = blockTable.value()["z"];
		}
	}
	else {
		DEBUG("ERROR: Inventory source '" + inventory + "' is not valid");
	}

	u16 offset = table["offset"].get<u16>();
	u16 count = table["count"].get<u16>();

	float width = 0, height = 0;
	sol::optional<sol::table> size = table["size"];
	if (size != sol::nullopt) {
		width = size.value()["x"];
		height = size.value()["y"];
	}

	m_data.inventoryWidgetList.emplace_back(); // LuaWidgetDef::InventoryWidget{{name, x, y}, player, inventory, width, height, offset, count});

	LuaWidgetDef::InventoryWidget &inv = m_data.inventoryWidgetList.back();
	inv.name = name;
	inv.x = x;
	inv.y = y;
	inv.inventory = inventory;
	inv.player = player;
	inv.inventory_name = inventory_name;
	inv.block = block;
	inv.width = width;
	inv.height = height;
	inv.offset = offset;
	inv.count = count;
}

void LuaGUI::addCraftingWidget(const sol::table &table) {
	// FIXME: Duplicated above
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	gk::Vector3i block;
	u16 offset = 0, size = 3;

	std::string inventory = table["inventory"].get<std::string>();
	if (inventory == "block") {
		sol::optional<sol::table> blockTable = table["block"];
		if (blockTable != sol::nullopt) {
			block.x = blockTable.value()["x"];
			block.y = blockTable.value()["y"];
			block.z = blockTable.value()["z"];
		}

		offset = table["offset"].get_or<u16>(0);
		size = table["size"].get_or<u16>(3);
	}
	else if (inventory == "temp") {
		size = table["size"].get_or<u16>(3);
	}
	else {
		DEBUG("ERROR: Inventory source '" + inventory + "' is not valid");
	}

	s32 resultX = 0, resultY = 0;
	sol::optional<sol::table> resultPosTable = table["result_pos"];
	if (resultPosTable != sol::nullopt) {
		resultX = resultPosTable.value()["x"];
		resultY = resultPosTable.value()["y"];
	}

	m_data.craftingWidgetList.emplace_back(); // LuaWidgetDef::CraftingWidget{{name, x, y}, block, offset, count});

	LuaWidgetDef::CraftingWidget &craftingWidget = m_data.craftingWidgetList.back();
	craftingWidget.name = name;
	craftingWidget.x = x;
	craftingWidget.y = y;
	craftingWidget.inventory = inventory;
	craftingWidget.block = block;
	craftingWidget.offset = offset;
	craftingWidget.size = size;
	craftingWidget.resultX = resultX;
	craftingWidget.resultY = resultY;
}

void LuaGUI::addProgressBarWidget(const sol::table &table) {
	// FIXME: Duplicated above
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	u8 type = table["type"].get_or<u8>(0);

	gk::Vector3i block;
	sol::optional<sol::table> blockTable = table["block"];
	if (blockTable != sol::nullopt) {
		block.x = blockTable.value()["x"];
		block.y = blockTable.value()["y"];
		block.z = blockTable.value()["z"];
	}
	else {
		DEBUG("ERROR: Attribute 'block' not defined for bar '" + name + "'");
		return;
	}

	std::string meta = table["meta"].get<std::string>();
	std::string maxMeta; u32 maxValue = table["max_value"].get_or<u32>(0);
	if (maxValue == 0) {
		maxMeta = table["max_meta"].get<std::string>();
	}

	gk::FloatRect clipRect;
	std::string texture = table["texture"].get<std::string>();
	sol::optional<sol::table> clipRectTable = table["clip"];
	if (clipRectTable != sol::nullopt) {
		clipRect.x = clipRectTable.value()["x"];
		clipRect.y = clipRectTable.value()["y"];
		clipRect.sizeX = clipRectTable.value()["width"];
		clipRect.sizeY = clipRectTable.value()["height"];
	}

	m_data.progressBarWidgetList.emplace_back();

	LuaWidgetDef::ProgressBarWidget &progressBarWidget = m_data.progressBarWidgetList.back();
	progressBarWidget.name = name;
	progressBarWidget.x = x;
	progressBarWidget.y = y;
	progressBarWidget.type = type;
	progressBarWidget.block = block;
	progressBarWidget.meta = meta;
	progressBarWidget.maxMeta = maxMeta;
	progressBarWidget.maxValue = maxValue;
	progressBarWidget.texture = texture;
	progressBarWidget.clipRect = clipRect;
}

void LuaGUI::show(Client &client) {
	sf::Packet packet;
	packet << Network::Command::BlockGUIData;

	for (auto &it : m_data.imageList)
		packet << u8(LuaWidget::Image)
			<< it.name << it.x << it.y << it.texture << it.clipRect.x << it.clipRect.y << it.clipRect.sizeX << it.clipRect.sizeY;

	for (auto &it : m_data.textButtonList)
		packet << u8(LuaWidget::TextButton) << it.name << it.x << it.y << it.text;

	for (auto &it : m_data.inventoryWidgetList)
		packet << u8(LuaWidget::InventoryWidget) << it.name << it.x << it.y
			<< it.inventory << it.player << it.inventory_name
			<< it.block.x << it.block.y << it.block.z
			<< it.width << it.height << it.offset << it.count;

	for (auto &it : m_data.craftingWidgetList)
		packet << u8(LuaWidget::CraftingWidget) << it.name << it.x << it.y << it.inventory
			<< it.block.x << it.block.y << it.block.z << it.offset << it.size << it.resultX << it.resultY;

	for (auto &it : m_data.progressBarWidgetList)
		packet << u8(LuaWidget::ProgressBarWidget) << it.name << it.x << it.y << it.type
			<< it.block.x << it.block.y << it.block.z << it.meta << it.maxMeta << it.maxValue
			<< it.texture << it.clipRect.x << it.clipRect.y << it.clipRect.sizeX << it.clipRect.sizeY;

	client.tcpSocket->send(packet);
}

void LuaGUI::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaGUI>("LuaGUI",
		"image",        &LuaGUI::addImage,
		"button",       &LuaGUI::addTextButton,
		"inventory",    &LuaGUI::addInventoryWidget,
		"crafting",     &LuaGUI::addCraftingWidget,
		"progress_bar", &LuaGUI::addProgressBarWidget,

		"show",         &LuaGUI::show
	);
}

