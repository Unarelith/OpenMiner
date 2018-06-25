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

void InventoryWidget::init(Inventory &inventory) {
	m_itemWidgets.clear();

	for (u16 y = 0 ; y < inventory.height() ; ++y) {
		for (u16 x = 0 ; x < inventory.width() ; ++x) {
			ItemWidget &widget = m_itemWidgets.emplace_back(inventory, x, y, this);
			widget.update();
			widget.setPosition(x * 18, y * 18, 0);
		}
	}

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.button.x / 3.0, event.button.y / 3.0)
			&& (!isReadOnly || mouseItemWidget.getStack().item().id() == 0)) {
				mouseItemWidget.swapItems(m_itemWidgets.at(i));
			}
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.button.x / 3.0, event.button.y / 3.0)
			&& !isReadOnly) {
				mouseItemWidget.putItem(m_itemWidgets.at(i));
			}
		}
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}
}

