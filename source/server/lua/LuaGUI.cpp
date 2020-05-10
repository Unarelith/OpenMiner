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
#include <gk/core/ApplicationStateStack.hpp>

#include "ClientInfo.hpp"
#include "LuaGUI.hpp"
#include "LuaWidget.hpp"
#include "Network.hpp"
#include "Registry.hpp"

#include "CraftingWidgetDef.hpp"
#include "ImageWidgetDef.hpp"
#include "InventoryWidgetDef.hpp"
#include "ProgressBarWidgetDef.hpp"
#include "ScrollBarWidgetDef.hpp"
#include "TextButtonWidgetDef.hpp"

void LuaGUI::addImage(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new ImageWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addTextButton(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new TextButtonWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addInventoryWidget(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new InventoryWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addCraftingWidget(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new CraftingWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addProgressBarWidget(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new ProgressBarWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addScrollBarWidget(const sol::table &table) {
	m_widgetDefinitions.emplace_back(new ScrollBarWidgetDef);
	m_widgetDefinitions.back()->loadFromLuaTable(table);
}

void LuaGUI::addInventory(const sol::table &table) {
	std::string name = table["name"].get<std::string>();

	u16 width = table["width"].get<u16>();
	u16 height = table["height"].get<u16>();

	m_inventoryList.emplace_back(width, height, name);

	Inventory &inventory = m_inventoryList.back();
	inventory.setUnlimited(table["is_unlimited"].get_or(false));

	sol::optional<sol::table> itemsTable = table["items"];
	if (itemsTable != sol::nullopt) {
		for (auto &it : itemsTable.value()) {
			std::string stringID = it.second.as<sol::table>()[1].get<std::string>();
			u16 amount = it.second.as<sol::table>()[2].get_or<u16>(1);

			inventory.addStack(stringID, amount);
		}
	}
}

void LuaGUI::show(ClientInfo &client) {
	Network::Packet packet;
	packet << Network::Command::BlockGUIData;

	packet << m_width << m_height << m_isCentered;

	for (auto &it : m_inventoryList)
		packet << u8(LuaWidget::Inventory) << it.name() << it;

	for (auto &it : m_widgetDefinitions)
		it->serialize(packet);

	client.tcpSocket->send(packet);
}

void LuaGUI::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaGUI>("LuaGUI",
		"image",          &LuaGUI::addImage,
		"button",         &LuaGUI::addTextButton,
		"inventory",      &LuaGUI::addInventoryWidget,
		"crafting",       &LuaGUI::addCraftingWidget,
		"progress_bar",   &LuaGUI::addProgressBarWidget,
		"scroll_bar",     &LuaGUI::addScrollBarWidget,
		"inventory_data", &LuaGUI::addInventory,

		"set_size",       &LuaGUI::setSize,
		"set_centered",   &LuaGUI::setCentered,

		"show",           &LuaGUI::show
	);
}

