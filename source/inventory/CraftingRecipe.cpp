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
#include "CraftingRecipe.hpp"

CraftingRecipe::CraftingRecipe(const std::array<u32, 9> &recipe, const ItemStack &result, bool isShapeless) {
	m_recipe = recipe;
	m_result = result;

	m_isShapeless = isShapeless;
}

bool CraftingRecipe::isMatching(const Inventory &inventory) const {
	if (!m_isShapeless) {
		for (u16 x = 0 ; x < 3 ; ++x) {
			for (u16 y = 0 ; y < 3 ; ++y) {
				if (m_recipe.at(x + y * 3) != inventory.getStack(x, y).item().id())
					return false;
			}
		}
		return true;
	}
	else {
		std::array<bool, 9> match{false, false, false, false, false, false, false, false, false};
		for (u32 id : m_recipe) {
			if (!id) continue;

			bool matched = false;
			for (u16 i = 0 ; i < 9 ; ++i) {
				if (inventory.getStack(i % 3, i / 3).item().id() == id && !match[i]) {
					match[i] = true;
					matched = true;
					break;
				}
			}

			if (!matched)
				return false;
		}
		return true;
	}
}

