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

#include "Types.hpp"

class Inventory {
	public:
		Inventory(u16 width, u16 height)
			: m_width(width), m_height(height) { m_items.resize(width * height); }

		u16 getItem(u16 x, u16 y) { return m_items.at(x + y * m_width); }
		void setItem(u16 x, u16 y, u16 id);

		void addItem(u16 id);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		const std::vector<u16> &items() const { return m_items; }

	private:
		u16 m_width = 0;
		u16 m_height = 0;

		std::vector<u16> m_items;
};

#endif // INVENTORY_HPP_
