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

CraftingWidget::CraftingWidget(Client &client, Inventory &craftingInventory, Widget *parent)
	: Widget(parent), m_client(client), m_craftingInventory(craftingInventory)
{
	m_craftingInventoryWidget.init(m_craftingInventory);
	m_craftingInventoryWidget.setPosition(29, 16, 0);

	m_craftingResultInventoryWidget.init(m_craftingResultInventory);
	m_craftingResultInventoryWidget.setPosition(123, 34, 0);
}

void CraftingWidget::onMouseEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget) {
	m_craftingInventoryWidget.onMouseEvent(event, mouseItemWidget);
	m_craftingResultInventoryWidget.onMouseEvent(event, mouseItemWidget, true);

	if (m_recipe && !m_craftingResultInventory.getStack(0, 0).item().id()) {
		for (u8 x = 0 ; x < m_craftingInventory.width() ; ++x) {
			for (u8 y = 0 ; y < m_craftingInventory.height() ; ++y) {
				const ItemStack &stack = m_craftingInventory.getStack(x, y);
				if (stack.item().id()) {
					m_craftingInventory.setStack(x, y, (stack.amount() > 1) ? stack.item().name() : "", stack.amount() - 1);
				}
			}
		}

		m_craftingInventoryWidget.init(m_craftingInventory);

		m_recipe = nullptr;
	}
}

void CraftingWidget::update() {
	const Recipe *recipe = Registry::getInstance().getRecipe(m_craftingInventory);
	if (recipe && recipe->type() != "craft")
		recipe = nullptr;

	if ((!m_recipe || m_recipe != recipe)) {
		m_recipe = recipe;

		if (m_recipe)
			m_craftingResultInventory.setStack(0, 0, m_recipe->result().item().name(), m_recipe->result().amount());
		else
			m_craftingResultInventory.setStack(0, 0, "", 0);

		m_craftingResultInventoryWidget.init(m_craftingResultInventory);
	}
}

void CraftingWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_craftingInventoryWidget, states);
	target.draw(m_craftingResultInventoryWidget, states);
}

