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

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.button.x / 3.0, event.button.y / 3.0) && (int)i != m_selectedItem) {
				if (m_selectedItem == -1 && m_itemWidgets[i].item() != 0) {
					m_selectedItem = i;

					m_itemWidgets.at(m_selectedItem).setPosition(event.button.x / 3 - getPosition().x - m_parent->getPosition().x - 8,
					                                             event.button.y / 3 - getPosition().y - m_parent->getPosition().y - 8, 0);
				}
				else if (m_selectedItem != -1) {
					m_itemWidgets.at(m_selectedItem).setPosition((i % m_inventoryWidth) * 18,
					                                             (i / m_inventoryWidth) * 18, 0);

					m_selectedItem = -1;
				}
			}
		}
	}

	if (event.type == SDL_MOUSEMOTION && m_selectedItem != -1) {
		m_itemWidgets.at(m_selectedItem).setPosition(event.motion.x / 3 - getPosition().x - m_parent->getPosition().x - 8,
		                                             event.motion.y / 3 - getPosition().y - m_parent->getPosition().y - 8, 0);
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		if ((int)i != m_selectedItem)
			target.draw(m_itemWidgets[i], states);
	}

	if (m_selectedItem != -1)
		target.draw(m_itemWidgets[m_selectedItem], states);
}

