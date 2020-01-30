/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/Exception.hpp>

#include "CraftingRecipe.hpp"
#include "SmeltingRecipe.hpp"
#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

Item &Registry::registerItem(const TilesDef &tiles, const std::string &id, const std::string &name) {
	u32 internalID = m_items.size();
	m_itemsID.emplace(id, internalID);
	m_items.emplace_back(internalID, tiles, id, name);
	return m_items.back();
}

Item &Registry::registerSerializedItem(sf::Packet &packet) {
	m_items.emplace_back();
	m_items.back().deserialize(packet);

	u32 internalID = m_items.size() - 1;
	m_itemsID.emplace(m_items.back().name(), internalID);

	return m_items.back();
}

const Block &Registry::getBlockFromStringID(const std::string &id) {
	if (id.empty()) return getBlock(0);
	auto it = m_blocksID.find(id);
	if (it == m_blocksID.end())
		throw EXCEPTION("Unknown block:", id);
	return getBlock(it->second);
}

const Item &Registry::getItemFromStringID(const std::string &id) {
	if (id.empty()) return getItem(0);
	auto it = m_itemsID.find(id);
	if (it == m_itemsID.end())
		throw EXCEPTION("Unknown item:", id);
	return getItem(it->second);
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

void Registry::serialize(sf::Packet &packet) {
	for (auto &it : m_items) {
		packet << u8(DataType::Item);
		it.serialize(packet);
	}

	for (auto &it : m_blocks) {
		packet << u8(DataType::Block);
		it->serialize(packet);
	}

	for (auto &it : m_recipes) {
		packet << u8((it->type() == "craft") ? DataType::CraftingRecipe : DataType::SmeltingRecipe);
		it->serialize(packet);
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

