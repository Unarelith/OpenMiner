/*
 * =====================================================================================
 *
 *       Filename:  CraftingRecipe.cpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 06:39:53
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <algorithm>

#include "CraftingRecipe.hpp"
#include "Exception.hpp"

// FIXME: Try to handle recipes a bit more like minecraft
//        - Pattern section | char[3][3]
//        - Key section     | map<char, vector<ItemType>>
//        - Result section  | ItemStack
CraftingRecipe::CraftingRecipe(const std::vector<std::string> &pattern, const std::map<char, std::vector<u32>> &keys, const ItemStack &result, bool isShapeless) {
	m_pattern = pattern;
	m_keys = keys;
	m_result = result;

	m_isShapeless = isShapeless;
}

bool CraftingRecipe::isMatching(const Inventory &inventory) const {
	if (inventory.height() < m_pattern.size())
		return false;

	if (!m_isShapeless) {
		for (u16 y = 0 ; y < inventory.height() - m_pattern.size() + 1 ; ++y) {
			if (inventory.width() < m_pattern.at(0).size())
				return false;

			for (u16 x = 0 ; x < inventory.width() - m_pattern.at(0).size() + 1 ; ++x)
				if (checkMatch(inventory, x, y))
					return true;
		}
		return false;
	}
	else {
		// std::array<bool, 9> match{false, false, false, false, false, false, false, false, false};
		// for (u32 id : m_recipe) {
		// 	bool matched = false;
		// 	for (u16 i = 0 ; i < 9 ; ++i) {
		// 		if (inventory.getStack(i % 3, i / 3).item().id() == id && !match[i]) {
		// 			match[i] = true;
		// 			matched = true;
		// 			break;
		// 		}
		// 	}
        //
		// 	if (!matched)
		// 		return false;
		// }
		// return true;
		return false; // FIXME
	}
}

bool CraftingRecipe::checkMatch(const Inventory &inventory, int offsetX, int offsetY) const {
	bool itemFound = false;
	u16 y = 0;
	for (y = 0 ; y < m_pattern.size() ; ++y) {
		u16 x = 0;
		for (x = 0 ; x < m_pattern[y].size() ; ++x) {
			itemFound = false;

			u32 inventoryItem = inventory.getStack(offsetX + x, offsetY + y).item().id();
			if (m_pattern[y][x] == ' ') {
				itemFound = !inventoryItem;
			}
			else {
				auto it = m_keys.find(m_pattern[y][x]);
				if (it == m_keys.end())
					throw EXCEPTION("Recipe error: Invalid key char(", (int)m_pattern[y][x], ")'");

				for (u32 item : it->second) {
					if (item == inventoryItem)
						itemFound = true;
				}
			}

			if (!itemFound)
				return false;
		}

		for (int x = offsetX - 1 ; x >= 0 ; --x) {
			if (inventory.getStack(x, offsetY + y).item().id())
				return false;
		}

		while (offsetX + x < inventory.width()) {
			if (inventory.getStack(offsetX + x, offsetY + y).item().id())
				return false;
			++x;
		}
	}

	for (int y = offsetY - 1 ; y >= 0 ; --y) {
		for (int x = 0 ; x < inventory.width() ; ++x) {
			if (inventory.getStack(x, y).item().id())
				return false;
		}
	}

	while (offsetY + y < inventory.height()) {
		for (int x = 0 ; x < inventory.width() ; ++x) {
			if (inventory.getStack(x, offsetY + y).item().id())
				return false;
		}
		++y;
	}

	return true;
}

