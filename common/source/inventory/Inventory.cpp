/*
 * =====================================================================================
 *
 *       Filename:  Inventory.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:03:48
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Inventory.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &name, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(name, amount);
}

void Inventory::addStack(const std::string &name, u16 amount) {
	for (std::size_t i = 0 ; i < m_items.size() ; ++i) {
		if (m_items[i].item().id() == 0) {
			m_items[i] = ItemStack(name, amount);
			break;
		}
		else if (m_items[i].item().name() == name) {
			m_items[i] = ItemStack(name, m_items[i].amount() + amount);
			break;
		}
	}
}

