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
#include <gk/core/Exception.hpp>

#include "CraftingRecipe.hpp"
#include "SmeltingRecipe.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

Item &Registry::registerItem(const TilesDef &tiles, const std::string &stringID, const std::string &label) {
	u32 id = m_items.size();
	m_itemsID.emplace(stringID, id);
	m_items.emplace_back(id, tiles, stringID, label);
	return m_items.back();
}

Item &Registry::registerSerializedItem(sf::Packet &packet) {
	m_items.emplace_back();
	m_items.back().deserialize(packet);

	u32 id = m_items.size() - 1;
	m_itemsID.emplace(m_items.back().stringID(), id);

	return m_items.back();
}

const Block &Registry::getBlockFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getBlock(0);
	auto it = m_blocksID.find(stringID);
	if (it == m_blocksID.end())
		throw EXCEPTION("Unknown block:", stringID);
	return getBlock(it->second);
}

const Item &Registry::getItemFromStringID(const std::string &stringID) {
	if (stringID.empty()) return getItem(0);
	auto it = m_itemsID.find(stringID);
	if (it == m_itemsID.end())
		throw EXCEPTION("Unknown item:", stringID);
	return getItem(it->second);
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

void Registry::serialize(sf::Packet &packet) const {
	for (auto &it : m_items) {
		packet << u8(DataType::Item) << it;
	}

	for (auto &it : m_blocks) {
		packet << u8(DataType::Block) << *it;
	}

	for (auto &it : m_recipes) {
		packet << u8((it->type() == "craft") ? DataType::CraftingRecipe : DataType::SmeltingRecipe)
			<< *it;
	}
}

void Registry::deserialize(sf::Packet &packet) {
	while (!packet.endOfPacket()) {
		u8 type;
		packet >> type;
		if (type == u8(DataType::Block)) {
			registerSerializedBlock<Block>(packet);
		}
		else if (type == u8(DataType::Item)) {
			registerSerializedItem(packet);
		}
		else if (type == u8(DataType::CraftingRecipe)) {
			registerRecipe<CraftingRecipe>()->deserialize(packet);
		}
		else if (type == u8(DataType::SmeltingRecipe)) {
			registerRecipe<SmeltingRecipe>()->deserialize(packet);
		}
	}
}

