/*
 * =====================================================================================
 *
 *       Filename:  ItemStack.cpp
 *
 *    Description:
 *
 *        Created:  28/12/2018 20:00:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemStack.hpp"
#include "Registry.hpp"

const Item &ItemStack::item() const {
	return Registry::getInstance().getItem(m_name);
}

