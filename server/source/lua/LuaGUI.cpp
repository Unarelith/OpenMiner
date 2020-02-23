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

	std::string player = table["player"].get<std::string>();
	std::string inventory = table["inventory"].get<std::string>();
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
	inv.player = player;
	inv.inventory = inventory;
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

	u16 offset = table["offset"].get_or<u16>(0);
	u16 size = table["size"].get_or<u16>(3);

	gk::Vector3i block;
	sol::optional<sol::table> blockTable = table["block"];
	if (blockTable != sol::nullopt) {
		block.x = blockTable.value()["x"];
		block.y = blockTable.value()["y"];
		block.z = blockTable.value()["z"];
	}

	float resultX = 0, resultY = 0;
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
	craftingWidget.block = block;
	craftingWidget.offset = offset;
	craftingWidget.size = size;
	craftingWidget.resultX = resultX;
	craftingWidget.resultY = resultY;
}

void LuaGUI::addFurnaceWidget(const sol::table &table) {
	// FIXME: Duplicated above
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	gk::Vector3i block;
	sol::optional<sol::table> blockTable = table["block"];
	if (blockTable != sol::nullopt) {
		block.x = blockTable.value()["x"];
		block.y = blockTable.value()["y"];
		block.z = blockTable.value()["z"];
	}

	m_data.furnaceWidgetList.emplace_back(); // LuaWidgetDef::FurnaceWidget{{name, x, y}, block});

	LuaWidgetDef::FurnaceWidget &furnaceWidget = m_data.furnaceWidgetList.back();
	furnaceWidget.name = name;
	furnaceWidget.x = x;
	furnaceWidget.y = y;
	furnaceWidget.block = block;
}

void LuaGUI::addPlayerCraftingWidget(const sol::table &table) {
	// FIXME: Duplicated above
	s32 x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	s32 resultX = 0, resultY = 0;
	sol::optional<sol::table> resultPosTable = table["result_pos"];
	if (resultPosTable != sol::nullopt) {
		resultX = resultPosTable.value()["x"];
		resultY = resultPosTable.value()["y"];
	}

	m_data.playerCraftingWidgetList.emplace_back();

	LuaWidgetDef::PlayerCraftingWidget &widget = m_data.playerCraftingWidgetList.back();
	widget.name = name;
	widget.x = x;
	widget.y = y;
	widget.resultX = resultX;
	widget.resultY = resultY;
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
			<< it.player << it.inventory << it.width << it.height << it.offset << it.count;
	for (auto &it : m_data.craftingWidgetList)
		packet << u8(LuaWidget::CraftingWidget) << it.name << it.x << it.y
			<< it.block.x << it.block.y << it.block.z << it.offset << it.size << it.resultX << it.resultY;
	for (auto &it : m_data.furnaceWidgetList)
		packet << u8(LuaWidget::FurnaceWidget) << it.name << it.x << it.y
			<< it.block.x << it.block.y << it.block.z;
	for (auto &it : m_data.playerCraftingWidgetList)
		packet << u8(LuaWidget::PlayerCraftingWidget) << it.name << it.x << it.y << it.resultX << it.resultY;
	client.tcpSocket->send(packet);
}

void LuaGUI::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaGUI>("LuaGUI",
		"image",     &LuaGUI::addImage,
		"button",    &LuaGUI::addTextButton,
		"inventory", &LuaGUI::addInventoryWidget,
		"crafting",  &LuaGUI::addCraftingWidget,
		"furnace",   &LuaGUI::addFurnaceWidget,
		"player_crafting", &LuaGUI::addPlayerCraftingWidget,
		"show",      &LuaGUI::show
	);
}

