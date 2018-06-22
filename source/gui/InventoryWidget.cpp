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

	for (u16 y = 0 ; y < inventory.height() ; ++y)
		for (u16 x = 0 ; x < inventory.width() ; ++x)
			m_itemWidgets.emplace_back(inventory.items().at(x + y * inventory.width()), this).setPosition(x * 18, y * 18, 0);

	// m_mouseItemWidget.setPosition(-10000, -10000, 0);

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.button.x / 3.0, event.button.y / 3.0)) {
				if (m_selectedItem == -1) {
					m_selectedItem = i;

					m_mouseItemWidget.setItem(m_itemWidgets.at(m_selectedItem).item());
					m_mouseItemWidget.setPosition(event.button.x / 3 - getPosition().x - m_parent->getPosition().x - 8,
					                          event.button.y / 3 - getPosition().y - m_parent->getPosition().y - 8, 0);

					m_itemWidgets.at(m_selectedItem).setItem(0);
				}
				else {
					u16 mouseItem = m_mouseItemWidget.item();
					m_mouseItemWidget.setItem(m_itemWidgets.at(i).item());
					m_itemWidgets.at(i).setItem(mouseItem);

					if (m_mouseItemWidget.item())
						m_selectedItem = i;
					else
						m_selectedItem = -1;
				}
			}
		}
	}

	if (event.type == SDL_MOUSEMOTION && m_selectedItem != -1) {
		m_mouseItemWidget.setPosition(event.motion.x / 3 - getPosition().x - m_parent->getPosition().x - 8,
		                              event.motion.y / 3 - getPosition().y - m_parent->getPosition().y - 8, 0);
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}

	target.draw(m_mouseItemWidget, states);
}

