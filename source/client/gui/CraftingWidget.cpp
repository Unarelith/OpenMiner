/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "CraftingWidget.hpp"
#include "Registry.hpp"

CraftingWidget::CraftingWidget(ClientCommandHandler &client, Inventory &craftingInventory, Widget *parent)
	: AbstractInventoryWidget(parent), m_client(client), m_craftingInventory(craftingInventory)
{
}

void CraftingWidget::init(unsigned int offset, unsigned int size) {
	m_craftingInventoryWidget.init(m_craftingInventory, offset, size * size);
	m_craftingResultInventoryWidget.init(m_craftingResultInventory);

	m_craftingInventoryWidget.setParent(this);
	m_craftingResultInventoryWidget.setParent(this);

	m_craftingInventoryWidget.setShiftDestination(m_shiftDestination);
	m_craftingResultInventoryWidget.setShiftDestination(m_shiftDestination);
}

void CraftingWidget::onEvent(const SDL_Event &event) {
	m_craftingInventoryWidget.onEvent(event);
	m_craftingResultInventoryWidget.onEvent(event);

	m_currentInventoryWidget = m_craftingResultInventoryWidget.currentItemWidget()
		? &m_craftingResultInventoryWidget : &m_craftingInventoryWidget;

	if (m_recipe && !m_craftingResultInventory.getStack(0, 0).item().id()) {
		for (u8 x = 0 ; x < m_craftingInventory.width() ; ++x) {
			for (u8 y = 0 ; y < m_craftingInventory.height() ; ++y) {
				const ItemStack &stack = m_craftingInventory.getStack(x, y);
				if (stack.item().id()) {
					m_craftingInventory.setStack(x, y, (stack.amount() > 1) ? stack.item().stringID() : "", stack.amount() - 1);
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
			m_craftingResultInventory.setStack(0, 0, m_recipe->result().item().stringID(), m_recipe->result().amount());
		else
			m_craftingResultInventory.setStack(0, 0, "", 0);

		m_craftingResultInventoryWidget.init(m_craftingResultInventory);
	}
}

bool CraftingWidget::sendItemStackToDest(const ItemWidget *itemStack, AbstractInventoryWidget *dest) {
	if (m_currentInventoryWidget) {
		return m_currentInventoryWidget->sendItemStackToDest(itemStack, dest);
	}

	return false;
}

bool CraftingWidget::receiveItemStack(const ItemWidget *itemStack, AbstractInventoryWidget *) {
	bool stackAdded = m_craftingInventory.addStack(itemStack->stack().item().stringID(), itemStack->stack().amount());

	if (stackAdded) {
		m_craftingInventoryWidget.update();
		m_craftingInventoryWidget.sendUpdatePacket();
	}

	return stackAdded;
}

void CraftingWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_craftingInventoryWidget, states);
	target.draw(m_craftingResultInventoryWidget, states);
}

