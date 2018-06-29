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
#include "WorkbenchWidget.hpp"

WorkbenchWidget::WorkbenchWidget(Inventory &playerInventory, Inventory &hotbarInventory, Inventory &craftingInventory, Widget *parent)
	: Widget(176, 166, parent), m_playerInventory(playerInventory), m_hotbarInventory(hotbarInventory), m_craftingWidget(craftingInventory, this)
{
	m_background.load("texture-workbench");
	m_background.setClipRect(0, 0, 176, 166);

	m_playerInventoryWidget.init(m_playerInventory);
	m_playerInventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_hotbarInventory);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);
}

void WorkbenchWidget::onEvent(const SDL_Event &event) {
	m_craftingWidget.onMouseEvent(event, m_mouseItemWidget);

	m_playerInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onMouseEvent(event, m_mouseItemWidget);

	m_mouseItemWidget.onEvent(event);
}

void WorkbenchWidget::update() {
	m_craftingWidget.update();

	const ItemWidget *currentItemWidget = nullptr;
	if ((currentItemWidget = m_playerInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_hotbarInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_craftingWidget.currentItemWidget()))
		m_mouseItemWidget.update(currentItemWidget);
	else
		m_mouseItemWidget.update(nullptr);
}

void WorkbenchWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);

	target.draw(m_craftingWidget, states);

	target.draw(m_playerInventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);

	target.draw(m_mouseItemWidget, states);
}

