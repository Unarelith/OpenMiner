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
#include <gk/system/Exception.hpp>

#include "Registry.hpp"

Registry *Registry::s_instance = nullptr;

Block &Registry::registerBlock(u32 textureID, const std::string &id, const std::string &name) {
	u32 internalID = m_blocks.size();
	m_blocksID.emplace(id, internalID);
	return m_blocks.emplace_back(internalID, textureID, id, name);
}

Item &Registry::registerItem(u32 textureID, const std::string &id, const std::string &name) {
	u32 internalID = m_items.size();
	m_itemsID.emplace(id, internalID);
	return m_items.emplace_back(internalID, textureID, id, name);
}

const Block &Registry::getBlock(const std::string &id) {
	if (id.empty()) return getBlock((int)0);
	auto it = m_blocksID.find(id);
	if (it == m_blocksID.end())
		throw EXCEPTION("Unknown block:", id);
	return getBlock(it->second);
}

const Item &Registry::getItem(const std::string &id) {
	if (id.empty()) return getItem((int)0);
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

