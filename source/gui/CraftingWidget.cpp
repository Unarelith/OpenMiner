/*
 * =====================================================================================
 *
 *       Filename:  CraftingWidget.cpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 02:27:35
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "CraftingWidget.hpp"
#include "Registry.hpp"

CraftingWidget::CraftingWidget(Widget *parent) : Widget(parent) {
	m_craftingInventoryWidget.init(m_craftingInventory);
	m_craftingInventoryWidget.setPosition(29, 16, 0);

	m_craftingResultInventoryWidget.init(m_craftingResultInventory);
	m_craftingResultInventoryWidget.setPosition(122, 34, 0);
}

void CraftingWidget::onEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget) {
	m_craftingInventoryWidget.onEvent(event, mouseItemWidget);
	m_craftingResultInventoryWidget.onEvent(event, mouseItemWidget);

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

void CraftingWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_craftingInventoryWidget, states);
	target.draw(m_craftingResultInventoryWidget, states);
}

