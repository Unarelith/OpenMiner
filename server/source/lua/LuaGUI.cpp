/*
 * =====================================================================================
 *
 *       Filename:  LuaGUI.cpp
 *
 *    Description:
 *
 *        Created:  06/01/2019 23:20:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>

#include "LuaGUI.hpp"
#include "Network.hpp"
// #include "LuaGUIState.hpp"

void LuaGUI::addImage(const sol::table &table) {
	// FIXME: Duplicated below
	float x = 0, y = 0;
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
		clipRect.width = clipRectTable.value()["width"];
		clipRect.height = clipRectTable.value()["height"];
	}

	m_data.imageList.emplace_back(LuaWidgetDef::Image{{name, x, y}, texture, clipRect});
}

void LuaGUI::addTextButton(const sol::table &table) {
	// FIXME: Duplicated below
	float x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	std::string text = table["text"].get<std::string>();
	sol::function on_click = table["on_click"].get<sol::function>();
	m_data.textButtonList.emplace_back(LuaWidgetDef::TextButton{{name, x, y}, text, on_click});
}

void LuaGUI::addInventoryWidget(const sol::table &table) {
	// FIXME: Duplicated above
	float x = 0, y = 0;
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

	m_data.inventoryWidgetList.emplace_back(LuaWidgetDef::InventoryWidget{{name, x, y},
			player, inventory, width, height, offset, count});
}

void LuaGUI::addCraftingWidget(const sol::table &table) {
	// FIXME: Duplicated above
	float x = 0, y = 0;
	sol::optional<sol::table> pos = table["pos"];
	std::string name = table["name"].get<std::string>();
	if (pos != sol::nullopt) {
		x = pos.value()["x"];
		y = pos.value()["y"];
	}

	u16 offset = table["offset"].get<u16>();
	u16 count = table["count"].get_or<u16>(9);

	gk::Vector3i block;
	sol::optional<sol::table> blockTable = table["block"];
	if (blockTable != sol::nullopt) {
		block.x = blockTable.value()["x"];
		block.y = blockTable.value()["y"];
		block.z = blockTable.value()["z"];
	}

	m_data.craftingWidgetList.emplace_back(LuaWidgetDef::CraftingWidget{{name, x, y},
			block, offset, count});
}

void LuaGUI::addFurnaceWidget(const sol::table &table) {
	// FIXME: Duplicated above
	float x = 0, y = 0;
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

	m_data.furnaceWidgetList.emplace_back(LuaWidgetDef::FurnaceWidget{{name, x, y}, block});
}

void LuaGUI::show(Client &client) {
	sf::Packet packet;
	packet << Network::Command::BlockGUIData;
	for (auto &it : m_data.imageList)
		packet << u8(0) << it.name << it.x << it.y << it.texture << it.clipRect.x << it.clipRect.y << it.clipRect.width << it.clipRect.height;
	for (auto &it : m_data.textButtonList)
		packet << u8(1) << it.name << it.x << it.y << it.text;
	for (auto &it : m_data.inventoryWidgetList)
		packet << u8(2) << it.name << it.x << it.y << it.player << it.inventory << it.width << it.height
			<< it.offset << it.count;
	for (auto &it : m_data.craftingWidgetList)
		packet << u8(3) << it.name << it.x << it.y << it.block.x << it.block.y << it.block.z
			<< it.offset << it.count;
	for (auto &it : m_data.furnaceWidgetList)
		packet << u8(4) << it.name << it.x << it.y << it.block.x << it.block.y << it.block.z;
	client.tcpSocket->send(packet);
}

void LuaGUI::initUsertype(sol::state &lua) {
	lua.new_usertype<LuaGUI>("LuaGUI",
		"image",     &LuaGUI::addImage,
		"button",    &LuaGUI::addTextButton,
		"inventory", &LuaGUI::addInventoryWidget,
		"crafting",  &LuaGUI::addCraftingWidget,
		"furnace",   &LuaGUI::addFurnaceWidget,
		"show",      &LuaGUI::show
	);
}

