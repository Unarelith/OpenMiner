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

	u16 i = 0;
	for (u16 id : inventory.items())
		m_itemWidgets.emplace_back(id).setPosition((++i - 1) * 18, 0, 0);

	setPosition(10.5, 86.5, 0);
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for (auto &it : m_itemWidgets)
		target.draw(it, states);
}

