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
#ifndef INVENTORYWIDGET_HPP_
#define INVENTORYWIDGET_HPP_

#include <gk/graphics/RectangleShape.hpp>

#include "MouseItemWidget.hpp"

class ClientCommandHandler;

class InventoryWidget : public Widget {
	public:
		InventoryWidget(ClientCommandHandler &client, Widget *parent = nullptr)
			: Widget(parent), m_client(client) {}

		void init(Inventory &inventory, unsigned int offset = 0, unsigned int size = 0);

		void onMouseEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly = false);

		const ItemWidget *currentItemWidget() const { return m_currentItemWidget; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void sendUpdatePacket();

		ClientCommandHandler &m_client;

		Inventory *m_inventory = nullptr;

		u16 m_inventoryWidth = 0;
		u16 m_inventoryHeight = 0;

		std::vector<ItemWidget> m_itemWidgets;
		ItemWidget *m_currentItemWidget = nullptr;

		gk::RectangleShape m_selectedItemBackground{16, 16, gk::Color{255, 255, 255, 80}};
};

#endif // INVENTORYWIDGET_HPP_
