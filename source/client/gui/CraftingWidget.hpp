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
#ifndef CRAFTINGWIDGET_HPP_
#define CRAFTINGWIDGET_HPP_

#include "InventoryWidget.hpp"

class Recipe;

class CraftingWidget : public AbstractInventoryWidget {
	public:
		CraftingWidget(ClientCommandHandler &client, Inventory &craftingInventory, Widget *parent = nullptr);

		void init(unsigned int offset = 0, unsigned int size = 3);

		void onEvent(const sf::Event &event) override;

		void update() override;

		bool sendItemStackToDest(const ItemWidget *itemStack, AbstractInventoryWidget *dest) override;
		bool receiveItemStack(const ItemWidget *itemStack, AbstractInventoryWidget *src) override;

		ItemWidget *currentItemWidget() const { return m_craftingResultInventoryWidget.currentItemWidget() ? m_craftingResultInventoryWidget.currentItemWidget() : m_craftingInventoryWidget.currentItemWidget(); }
		InventoryWidget *currentInventoryWidget() const { return m_currentInventoryWidget; }

		InventoryWidget &craftingInventoryWidget() { return m_craftingInventoryWidget; }
		InventoryWidget &craftingResultInventoryWidget() { return m_craftingResultInventoryWidget; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ClientCommandHandler &m_client;

		InventoryWidget *m_currentInventoryWidget = nullptr;

		Inventory &m_craftingInventory;
		InventoryWidget m_craftingInventoryWidget{m_client};

		Inventory m_craftingResultInventory{1, 1};
		InventoryWidget m_craftingResultInventoryWidget{m_client, true};

		const Recipe *m_recipe = nullptr;
};

#endif // CRAFTINGWIDGET_HPP_
