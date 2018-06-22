/*
 * =====================================================================================
 *
 *       Filename:  WorkbenchWidget.cpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:22:29
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "WorkbenchWidget.hpp"

WorkbenchWidget::WorkbenchWidget(Widget *parent) : Widget(176, 166, parent) {
	m_background.load("texture-workbench");
	m_background.setClipRect(0, 0, 176, 166);

	for (u16 i = 0 ; i < 12 ; ++i) {
		if (i < 9)
			m_hotbarInventory.setItem(i, 0, i + 1);
		else
			m_inventory.setItem(i - 9, 0, i + 1);
	}

	m_craftingInventoryWidget.init(m_craftingInventory);
	m_craftingInventoryWidget.setPosition(29, 16, 0);

	m_inventoryWidget.init(m_inventory);
	m_inventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_hotbarInventory);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);

	// setScale(3, 3, 1);
	setPosition(SCREEN_WIDTH  / 3.0 / 2.0 - m_background.clipRect().width  / 2.0,
	            SCREEN_HEIGHT / 3.0 / 2.0 - m_background.clipRect().height / 2.0, 0);
}

void WorkbenchWidget::onEvent(const SDL_Event &event) {
	m_craftingInventoryWidget.onEvent(event, m_mouseItemWidget);
	m_inventoryWidget.onEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onEvent(event, m_mouseItemWidget);

	m_mouseItemWidget.onEvent(event);
}

void WorkbenchWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);

	target.draw(m_craftingInventoryWidget, states);
	target.draw(m_inventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);

	target.draw(m_mouseItemWidget, states);
}

