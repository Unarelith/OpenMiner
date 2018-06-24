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
#include "Registry.hpp"
#include "WorkbenchWidget.hpp"

WorkbenchWidget::WorkbenchWidget(Inventory &playerInventory, Inventory &hotbarInventory, Widget *parent)
	: Widget(176, 166, parent), m_playerInventory(playerInventory), m_hotbarInventory(hotbarInventory)
{
	m_background.load("texture-workbench");
	m_background.setClipRect(0, 0, 176, 166);

	m_craftingInventoryWidget.init(m_craftingInventory);
	m_craftingInventoryWidget.setPosition(29, 16, 0);

	m_craftingResultInventoryWidget.init(m_craftingResultInventory);
	m_craftingResultInventoryWidget.setPosition(122, 34, 0);

	m_playerInventoryWidget.init(m_playerInventory);
	m_playerInventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_hotbarInventory);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);

	// setScale(3, 3, 1);
	setPosition(SCREEN_WIDTH  / 3.0 / 2.0 - m_background.clipRect().width  / 2.0,
	            SCREEN_HEIGHT / 3.0 / 2.0 - m_background.clipRect().height / 2.0, 0);
}

#include "Debug.hpp"
void WorkbenchWidget::onEvent(const SDL_Event &event) {
	m_craftingInventoryWidget.onEvent(event, m_mouseItemWidget);
	m_craftingResultInventoryWidget.onEvent(event, m_mouseItemWidget);
	m_playerInventoryWidget.onEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onEvent(event, m_mouseItemWidget);

	m_mouseItemWidget.onEvent(event);

	const CraftingRecipe *recipe = Registry::getInstance().getRecipe(m_craftingInventory);
	if (recipe != nullptr && !m_craftingResultInventory.getStack(0, 0).item().id()) {
		m_craftingResultInventory.setStack(0, 0, recipe->result().item().id(), recipe->result().amount());
		m_craftingResultInventoryWidget.init(m_craftingResultInventory);

		// FIXME: Make a subclass CraftingInventory to handle source items and crafting result
		//        Source elements should be destroyed if crafting result is taken
		//        Crafting result should be read-only
		for (u8 i = 0 ; i < 9 ; ++i) {
			const ItemStack &stack = m_craftingInventory.getStack(i % 3, i / 3);
			if (stack.item().id()) {
				m_craftingInventory.setStack(i % 3, i / 3, (stack.amount() > 1) ? stack.item().id() : 0, stack.amount() - 1);
			}
		}
		m_craftingInventoryWidget.init(m_craftingInventory);
	}
}

void WorkbenchWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);

	target.draw(m_craftingInventoryWidget, states);
	target.draw(m_craftingResultInventoryWidget, states);
	target.draw(m_playerInventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);

	target.draw(m_mouseItemWidget, states);
}
