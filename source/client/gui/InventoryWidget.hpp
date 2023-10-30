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
#ifndef INVENTORYWIDGET_HPP_
#define INVENTORYWIDGET_HPP_

#include "AbstractInventoryWidget.hpp"
#include "MouseItemWidget.hpp"
#include "RectangleShape.hpp"

class ClientCommandHandler;

class InventoryWidget : public AbstractInventoryWidget {
	public:
		InventoryWidget(ClientCommandHandler &client, bool isReadOnly = false, Widget *parent = nullptr)
			: AbstractInventoryWidget(parent, isReadOnly), m_client(client) {}

		void init(Inventory &inventory, u16 offset = 0, u16 size = 0);

		void scroll(float scrolling);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		void applySearch(const std::string &search);

		bool sendItemStackToDest(const ItemWidget *itemStack, AbstractInventoryWidget *dest) override;
		ItemStack receiveItemStack(const ItemWidget *itemStack, AbstractInventoryWidget *src) override;

		void sendUpdatePacket();

		Inventory *inventory() const { return m_inventory; }

		ItemWidget *currentItemWidget() const { return m_currentItemWidget; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		void loadItemWidgets(u16 offset, u16 size);
		void updateItemWidgets(u16 offset, u16 size, std::string search = "");

		ClientCommandHandler &m_client;

		Inventory *m_inventory = nullptr;

		u16 m_inventoryWidth = 0;
		u16 m_inventoryHeight = 0;

		u16 m_offset = 0;
		u16 m_size = 0;

		std::vector<ItemWidget> m_itemWidgets;
		ItemWidget *m_currentItemWidget = nullptr;

		RectangleShape m_selectedItemBackground{16, 16, Color::fromRGBA32(255, 255, 255, 80)};

		std::string m_lastSearch;
};

#endif // INVENTORYWIDGET_HPP_
