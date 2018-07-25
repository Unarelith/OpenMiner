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

void InventoryWidget::init(Inventory &inventory, unsigned int offset, unsigned int size) {
	m_itemWidgets.clear();

	// for (u16 y = 0 ; y < inventory.height() ; ++y) {
	// 	for (u16 x = 0 ; x < inventory.width() ; ++x) {
	for (u16 i = 0 ; i < (size > 0 ? size : inventory.width() * inventory.height()) ; ++i) {
		ItemWidget &widget = m_itemWidgets.emplace_back(inventory, (i + offset) % inventory.width(), (i + offset) / inventory.width(), this);
		widget.update();
		widget.setPosition((i % inventory.width()) * 18, (i / inventory.width()) * 18, 0);
	}

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onMouseEvent(const sf::Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly) {
	if (event.type == sf::Event::MouseMoved) {
		m_currentItemWidget = nullptr;
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.mouseMove.x, event.mouseMove.y)) {
				m_currentItemWidget = &m_itemWidgets[i];

				m_selectedItemBackground.setPosition(1 + (i % m_inventoryWidth) * 18, 1 + (i / m_inventoryWidth) * 18, 0);
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_currentItemWidget) {
		mouseItemWidget.swapItems(*m_currentItemWidget, isReadOnly);
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && m_currentItemWidget) {
		if (!isReadOnly) {
			mouseItemWidget.putItem(*m_currentItemWidget);
		}
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}

	if (m_currentItemWidget)
		target.draw(m_selectedItemBackground, states);
}

