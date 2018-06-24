/*
 * =====================================================================================
 *
 *       Filename:  ItemStack.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:23:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMSTACK_HPP_
#define ITEMSTACK_HPP_

#include "Item.hpp"

class ItemStack {
	public:
		ItemStack();
		ItemStack(u16 id, u16 amount = 1);

		const Item &item() const { return *m_item; }

		u16 amount() const { return m_amount; }
		void setAmount(u16 amount) { m_amount = amount; }

	private:
		const Item *m_item = nullptr;

		u16 m_amount = 0;
};

#endif // ITEMSTACK_HPP_
