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
#include "ClientCommandHandler.hpp"
#include "InventoryWidget.hpp"

void InventoryWidget::init(Inventory &inventory, unsigned int offset, unsigned int size) {
	m_inventory = &inventory;

	m_itemWidgets.clear();

	for (u16 i = 0 ; i < (size > 0 ? size : inventory.width() * inventory.height()) ; ++i) {
		m_itemWidgets.emplace_back(inventory, (i + offset) % inventory.width(), (i + offset) / inventory.width(), this);

		ItemWidget &widget = m_itemWidgets.back();
		widget.update();
		widget.setPosition((i % inventory.width()) * 18, (i / inventory.width()) * 18, 0);
	}

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onMouseEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly) {
	if (event.type == SDL_MOUSEMOTION) {
		m_currentItemWidget = nullptr;
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
			if (m_itemWidgets[i].isPointInWidget(event.motion.x, event.motion.y)) {
				m_currentItemWidget = &m_itemWidgets[i];

				m_selectedItemBackground.setPosition(1 + (i % m_inventoryWidth) * 18, 1 + (i / m_inventoryWidth) * 18, 0);
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && m_currentItemWidget) {
		mouseItemWidget.swapItems(*m_currentItemWidget, isReadOnly);

		sendUpdatePacket();
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && m_currentItemWidget) {
		if (!isReadOnly) {
			mouseItemWidget.putItem(*m_currentItemWidget);

			sendUpdatePacket();
		}
	}
}

void InventoryWidget::sendUpdatePacket() {
	if (m_inventory->inBlock()) {
		m_client.sendBlockInvUpdate(*m_inventory);
	}
	else {
		m_client.sendPlayerInvUpdate();
	}
}

void InventoryWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}

	if (m_currentItemWidget)
		target.draw(m_selectedItemBackground, states);
}

