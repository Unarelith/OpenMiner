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

CraftingRecipe::CraftingRecipe(const std::array<u32, 9> &recipe, const ItemStack &result) {
	m_recipe = recipe;
	m_result = result;
}

bool CraftingRecipe::isMatching(const Inventory &inventory) const {
	for (u16 x = 0 ; x < 3 ; ++x) {
		for (u16 y = 0 ; y < 3 ; ++y) {
			if (m_recipe.at(x + y * 3) != inventory.getStack(x, y).item().id())
				return false;
		}
	}
	return true;
}

