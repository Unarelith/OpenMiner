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

void Inventory::setItem(u16 x, u16 y, u16 id) {
	m_items[x + y * m_width] = id;
}

