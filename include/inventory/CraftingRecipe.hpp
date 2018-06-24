/*
 * =====================================================================================
 *
 *       Filename:  CraftingRecipe.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 06:38:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CRAFTINGRECIPE_HPP_
#define CRAFTINGRECIPE_HPP_

#include <array>

#include "Inventory.hpp"

class CraftingRecipe {
	public:
		CraftingRecipe(const std::array<u32, 9> &recipe, const ItemStack &result);

		bool isMatching(const Inventory &inventory) const;

		const ItemStack &result() const { return m_result; }

	private:
		std::array<u32, 9> m_recipe;
		ItemStack m_result;
};

#endif // CRAFTINGRECIPE_HPP_
