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

void Inventory::addItem(u16 id) {
	m_items.emplace_back(id);
}

