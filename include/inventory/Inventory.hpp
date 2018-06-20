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
			: m_width(width), m_height(height) {}

		void addItem(u16 id);

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		const std::vector<u16> &items() const { return m_items; }

	private:
		u16 m_width;
		u16 m_height;

		std::vector<u16> m_items;
};

#endif // INVENTORY_HPP_
