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
#ifndef HOTBAR_HPP_
#define HOTBAR_HPP_

#include <vector>

#include "common/inventory/Inventory.hpp"
#include "client/gui/ItemWidget.hpp"

class ClientCommandHandler;
class ClientPlayer;

class Hotbar : public Widget {
	public:
		Hotbar(ClientPlayer &player, ClientCommandHandler &client, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		int cursorPos() const { return m_cursorPos; }
		const Item &currentItem() const;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Image m_cursor;
		int m_cursorPos = -1;

		ClientPlayer &m_player;

		ClientCommandHandler &m_client;

		std::vector<ItemWidget> m_items;
};

#endif // HOTBAR_HPP_
