/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "PlayerInventoryWidget.hpp"

PlayerInventoryWidget::PlayerInventoryWidget(ClientCommandHandler &client, MouseItemWidget &mouseItemWidget, Inventory &playerInventory, Widget *parent)
	: Widget(176, 166, parent), m_client(client), m_mouseItemWidget(mouseItemWidget), m_playerInventory(playerInventory)
{
	m_background.load("texture-inventory");
	m_background.setClipRect(0, 0, 176, 166);

	m_playerInventoryWidget.init(m_playerInventory, 9, 9 * 3);
	m_playerInventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_playerInventory, 0, 9);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);

	m_craftingWidget.craftingInventoryWidget().setPosition(97, 17, 0);
	m_craftingWidget.craftingInventoryWidget().init(m_craftingInventory, 0, 4);
	m_craftingWidget.craftingResultInventoryWidget().setPosition(153, 27, 0);
}

void PlayerInventoryWidget::onEvent(const SDL_Event &event) {
	m_craftingWidget.onMouseEvent(event, m_mouseItemWidget);

	m_playerInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
}

void PlayerInventoryWidget::update() {
	m_craftingWidget.update();

	const ItemWidget *currentItemWidget = nullptr;
	if ((currentItemWidget = m_playerInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_hotbarInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_craftingWidget.currentItemWidget()))
		m_mouseItemWidget.updateCurrentItem(currentItemWidget);
	else
		m_mouseItemWidget.updateCurrentItem(nullptr);
}

void PlayerInventoryWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);

	target.draw(m_craftingWidget, states);

	target.draw(m_playerInventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);
}

