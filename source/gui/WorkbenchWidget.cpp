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

	for (u16 i = 0 ; i < 12 ; ++i)
		m_inventory.setItem(i % m_inventory.width(), i / m_inventory.width(), i + 1);

	m_inventoryWidget.update(m_inventory);
	m_inventoryWidget.setPosition(7, 86, 0);

	// setScale(3, 3, 1);
	setPosition(SCREEN_WIDTH  / 3.0 / 2.0 - m_background.clipRect().width  / 2.0,
	            SCREEN_HEIGHT / 3.0 / 2.0 - m_background.clipRect().height / 2.0, 0);
}

void WorkbenchWidget::onEvent(const SDL_Event &event) {
	m_inventoryWidget.onEvent(event);
}

void WorkbenchWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);
	target.draw(m_inventoryWidget, states);
}

