/*
 * =====================================================================================
 *
 *       Filename:  InventoryWidget.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:09:20
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "InventoryWidget.hpp"

void InventoryWidget::update(const Inventory &inventory) {
	m_itemWidgets.clear();

	for (u16 id : inventory.items()) {
		// TODO: Set the right position
		m_itemWidgets.emplace_back(id);
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	for (auto &it : m_itemWidgets)
		target.draw(it, states);
}

