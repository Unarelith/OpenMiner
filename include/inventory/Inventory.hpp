/*
 * =====================================================================================
 *
 *       Filename:  Inventory.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 00:59:26
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INVENTORY_HPP_
#define INVENTORY_HPP_

#include <vector>

#include "ItemStack.hpp"

class Inventory {
	public:
		Inventory(u16 width, u16 height)
			: m_width(width), m_height(height) { m_items.resize(width * height); }

		const ItemStack &getStack(u16 x, u16 y) const { return m_items.at(x + y * m_width); }
		ItemStack &getStackRef(u16 x, u16 y) { return m_items.at(x + y * m_width); }
		void setStack(u16 x, u16 y, u16 id, u16 amount = 1);
		void addStack(u16 id, u16 amount = 1);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

	private:
		u16 m_width = 0;
		u16 m_height = 0;

		std::vector<ItemStack> m_items;
};

#endif // INVENTORY_HPP_
