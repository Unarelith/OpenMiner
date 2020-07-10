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
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();

	ServerBlock &block = Registry::getInstance().registerBlock<ServerBlock>(stringID);
	block.setRotatable(table["is_rotatable"].get_or(false));

	BlockState &defaultState = block.getState(0);
	loadBlockState(defaultState, table, block);
}

void LuaBlockLoader::loadBlockState(BlockState &state, const sol::table &table, ServerBlock &block) const {
	TilesDef tiles;
	tiles.loadFromLuaTable(table);
	state.tiles(tiles);

	loadProperties(state, table);
	loadBoundingBox(state, table);
	loadDrawType(state, table, block);
	loadItemDrop(state, table);
	loadColorMultiplier(state, table);

	Item *item = nullptr;
	if (!state.inventoryImage().empty()) {
		item = &Registry::getInstance().registerItem<Item>(TilesDef{state.inventoryImage()}, block.stringID(), state.label());
	}
	else {
		item = &Registry::getInstance().registerItem<Item>(state.tiles(), block.stringID(), state.label());
	}

	item->setIsBlock(true);

	loadGroups(block, table, item);

	loadParams(block);
}

inline void LuaBlockLoader::loadProperties(BlockState &state, const sol::table &table) const {
	state.label(table["name"].get<std::string>());
	state.harvestRequirements(table["harvest_requirements"].get_or(0));
	state.hardness(table["hardness"].get_or(1.0f));
	state.isOpaque(table["is_opaque"].get_or(true));
	state.isLightSource(table["is_light_source"].get_or(false));
	state.inventoryImage(table["inventory_image"].get_or<std::string>(""));
	state.fogDepth(table["fog_depth"].get_or<float>(0));

	// state.onBlockActivated(table["on_block_activated"]);
	// state.onTick(table["on_tick"]);
	// state.onBlockPlaced(table["on_block_placed"]);
	// state.onBlockDestroyed(table["on_block_destroyed"]);
	// state.setTickRandomly(table["tick_randomly"].get_or(false));
	// state.setTickProbability(table["tick_probability"].get_or(0.f));

	if (state.fogDepth()) {
		sol::optional<sol::table> fogColor = table["fog_color"];
		if (fogColor != sol::nullopt) {
			state.fogColor(gk::Color{
				fogColor.value().get<u8>(1),
				fogColor.value().get<u8>(2),
				fogColor.value().get<u8>(3),
			});
		}
	}
}

inline void LuaBlockLoader::loadBoundingBox(BlockState &state, const sol::table &table) const {
	sol::optional<sol::table> boundingBox = table["bounding_box"];
	if (boundingBox != sol::nullopt) {
		state.boundingBox(gk::FloatBox{
			boundingBox.value().get<float>(1),
			boundingBox.value().get<float>(2),
			boundingBox.value().get<float>(3),
			boundingBox.value().get<float>(4),
			boundingBox.value().get<float>(5),
			boundingBox.value().get<float>(6),
		});
	}
}

inline void LuaBlockLoader::loadDrawType(BlockState &state, const sol::table &table, const ServerBlock &block) const {
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
			if (it != drawTypes.end()) {
				state.drawType(it->second);
			}
			else
				gkError() << "In" << block.stringID() << " definition: Block draw type invalid";
		}
		else
			gkError() << "In" << block.stringID() << " definition: Block draw type must be a string";
	}
}

inline void LuaBlockLoader::loadItemDrop(BlockState &state, const sol::table &table) const {
	sol::optional<sol::table> itemDrop = table["item_drop"];
	if (itemDrop != sol::nullopt) {
		state.itemDrop(itemDrop.value()["id"]);
		state.itemDropAmount(itemDrop.value()["amount"]);
	}
}

inline void LuaBlockLoader::loadColorMultiplier(BlockState &state, const sol::table &table) const {
	sol::optional<sol::table> colorMultiplier = table["color_multiplier"];
	if (colorMultiplier != sol::nullopt) {
		state.colorMultiplier(gk::Color{
			colorMultiplier.value().get<u8>(1),
			colorMultiplier.value().get<u8>(2),
			colorMultiplier.value().get<u8>(3),
			colorMultiplier.value().get<u8>(4)
		});
	}
}

inline void LuaBlockLoader::loadGroups(ServerBlock &block, const sol::table &table, Item *item) const {
	sol::object groupsObject = table["groups"];
	if (groupsObject.valid()) {
		if (groupsObject.get_type() == sol::type::table) {
			sol::table groupsTable = groupsObject.as<sol::table>();
			for (auto &groupObject : groupsTable) {
				std::string groupName = "group:" + groupObject.first.as<std::string>();
				u16 groupValue = groupObject.second.as<u16>();

				block.addGroup(groupName, groupValue);
				if (item)
					item->addGroup(groupName, groupValue);
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

