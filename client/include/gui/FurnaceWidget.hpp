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
#ifndef FURNACEWIDGET_HPP_
#define FURNACEWIDGET_HPP_

#include "BlockData.hpp"
#include "InventoryWidget.hpp"

class FurnaceWidget : public Widget {
	public:
		FurnaceWidget(ClientCommandHandler &client, MouseItemWidget &mouseItemWidget, Inventory &playerInventory, BlockData &blockData, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ClientCommandHandler &m_client;

		gk::Image m_background;

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{m_client, this};
		InventoryWidget m_hotbarInventoryWidget{m_client, this};

		InventoryWidget m_inputInventoryWidget{m_client, this};
		InventoryWidget m_outputInventoryWidget{m_client, this};
		InventoryWidget m_fuelInventoryWidget{m_client, this};

		BlockData &m_blockData;

		gk::Image m_burnImage{"texture-furnace"};
		gk::Image m_progressImage{"texture-furnace"};

		MouseItemWidget &m_mouseItemWidget;
};

#endif // FURNACEWIDGET_HPP_
