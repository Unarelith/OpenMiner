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
#ifndef PLAYERINVENTORYWIDGET_HPP_
#define PLAYERINVENTORYWIDGET_HPP_

#include "CraftingWidget.hpp"

class PlayerInventoryWidget : public Widget {
	public:
		PlayerInventoryWidget(ClientCommandHandler &client, MouseItemWidget &mouseItemWidget, Inventory &playerInventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ClientCommandHandler &m_client;

		MouseItemWidget &m_mouseItemWidget;

		gk::Image m_background;

		Inventory m_craftingInventory{2, 2};
		CraftingWidget m_craftingWidget{m_client, m_craftingInventory, this};

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{m_client, this};
		InventoryWidget m_hotbarInventoryWidget{m_client, this};
};

#endif // PLAYERINVENTORYWIDGET_HPP_
