/*
 * =====================================================================================
 *
 *       Filename:  ItemStack.cpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:23:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemStack.hpp"
#include "Registry.hpp"

ItemStack::ItemStack() : m_item(&Registry::getInstance().getItem(0)) {
}

ItemStack::ItemStack(u16 id, u16 amount) : m_item(&Registry::getInstance().getItem(id)) {
	m_amount = amount;
}

