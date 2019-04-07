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
#include "Client.hpp"
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
	sf::Packet packet;
	if (m_inventory->inBlock()) {
		packet << Network::Command::BlockInvUpdate;
		packet << s32(m_inventory->blockPos().x) << s32(m_inventory->blockPos().y) << s32(m_inventory->blockPos().z);
		packet << *m_inventory;
	}
	else {
		packet << Network::Command::PlayerInvUpdate << m_client.id();
		packet << *m_inventory;
	}
	m_client.send(packet);
}

void InventoryWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}

	if (m_currentItemWidget)
		target.draw(m_selectedItemBackground, states);
}

