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
#include <gk/core/Debug.hpp>

#include "LuaBlockLoader.hpp"
#include "LuaMod.hpp"
#include "Registry.hpp"
#include "ServerBlock.hpp"

void LuaBlockLoader::loadBlock(const sol::table &table) const {
	TilesDef tiles;
	tiles.loadFromLuaTable(table);

	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();
	std::string label = table["name"].get<std::string>();

	ServerBlock &block = Registry::getInstance().registerBlock<ServerBlock>(tiles, stringID, label);

	loadProperties(block, table);
	loadBoundingBox(block, table);
	loadDrawType(block, table);
	loadItemDrop(block, table);
	loadColorMultiplier(block, table);

	Item *item = nullptr;
	if (!block.inventoryImage().empty()) {
		item = &Registry::getInstance().registerItem<Item>(TilesDef{block.inventoryImage()}, stringID, label);
	}
	else {
		item = &Registry::getInstance().registerItem<Item>(block.tiles(), stringID, label);
	}

	item->setIsBlock(true);

	loadGroups(block, *item, table);

	loadParams(block);
}

inline void LuaBlockLoader::loadProperties(ServerBlock &block, const sol::table &table) const {
	block.setHarvestRequirements(table["harvest_requirements"].get_or(0));
	block.setHardness(table["hardness"].get_or(1.0f));
	block.setOpaque(table["is_opaque"].get_or(true));
	block.setLightSource(table["is_light_source"].get_or(false));
	block.setOnBlockActivated(table["on_block_activated"]);
	block.setOnTick(table["on_tick"]);
	block.setOnBlockPlaced(table["on_block_placed"]);
	block.setOnBlockDestroyed(table["on_block_destroyed"]);
	block.setRotatable(table["is_rotatable"].get_or(false));
	block.setInventoryImage(table["inventory_image"].get_or<std::string>(""));
	block.setFogDepth(table["fog_depth"].get_or<float>(0));
	block.setTickRandomly(table["tick_randomly"].get_or(false));
	block.setTickProbability(table["tick_probability"].get_or(0.f));

	if (block.fogDepth()) {
		sol::optional<sol::table> fogColor = table["fog_color"];
		if (fogColor != sol::nullopt) {
			block.setFogColor(gk::Color{
				fogColor.value().get<u8>(1),
				fogColor.value().get<u8>(2),
				fogColor.value().get<u8>(3),
			});
		}
	}
}

inline void LuaBlockLoader::loadBoundingBox(ServerBlock &block, const sol::table &table) const {
	sol::optional<sol::table> boundingBox = table["bounding_box"];
	if (boundingBox != sol::nullopt) {
		block.setBoundingBox(gk::FloatBox{
			boundingBox.value().get<float>(1),
			boundingBox.value().get<float>(2),
			boundingBox.value().get<float>(3),
			boundingBox.value().get<float>(4),
			boundingBox.value().get<float>(5),
			boundingBox.value().get<float>(6),
		});
	}
}

inline void LuaBlockLoader::loadDrawType(ServerBlock &block, const sol::table &table) const {
	sol::object drawTypeObject = table["draw_type"];
	if (drawTypeObject.valid()) {
		if (drawTypeObject.get_type() == sol::type::string) {
			static const std::unordered_map<std::string, BlockDrawType> drawTypes = {
				{"solid",  BlockDrawType::Solid},
				{"xshape", BlockDrawType::XShape},
				{"leaves", BlockDrawType::Leaves},
				{"liquid", BlockDrawType::Liquid},
				{"glass",  BlockDrawType::Glass},
				{"cactus", BlockDrawType::Cactus},
				{"boundingbox", BlockDrawType::BoundingBox}, // FIXME: Temporary
			};

			auto it = drawTypes.find(drawTypeObject.as<std::string>());
			if (it != drawTypes.end())
				block.setDrawType(it->second);
			else
				gkError() << "In" << block.stringID() << " definition: Block draw type invalid";
		}
		else
			gkError() << "In" << block.stringID() << " definition: Block draw type must be a string";
	}
}

inline void LuaBlockLoader::loadItemDrop(ServerBlock &block, const sol::table &table) const {
	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		std::string dropID = itemDrop.value()["id"];
		u16 dropAmount = itemDrop.value()["amount"];
		block.setItemDrop(dropID, dropAmount);
	}
}

inline void LuaBlockLoader::loadColorMultiplier(ServerBlock &block, const sol::table &table) const {
	sol::optional<sol::table> colorMultiplier = table["color_multiplier"];
	if (colorMultiplier != sol::nullopt) {
		block.setColorMultiplier(gk::Color{
			colorMultiplier.value().get<u8>(1),
			colorMultiplier.value().get<u8>(2),
			colorMultiplier.value().get<u8>(3),
			colorMultiplier.value().get<u8>(4)
		});
	}
}

inline void LuaBlockLoader::loadGroups(ServerBlock &block, Item &item, const sol::table &table) const {
	sol::object groupsObject = table["groups"];
	if (groupsObject.valid()) {
		if (groupsObject.get_type() == sol::type::table) {
			sol::table groupsTable = groupsObject.as<sol::table>();
			for (auto &groupObject : groupsTable) {
				std::string groupName = "group:" + groupObject.first.as<std::string>();
				u16 groupValue = groupObject.second.as<u16>();

				block.addGroup(groupName, groupValue);
				item.addGroup(groupName, groupValue);
			}
		}
		else
			gkError() << "For block" << block.stringID() << ": 'groups' should be a table";
	}
}

void LuaBlockLoader::loadParams(ServerBlock &block) const {
	if (block.isRotatable())
		block.param().allocateBits(BlockParam::Type::Rotation, 5);
}

