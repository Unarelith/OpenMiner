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
#include "ClientCommandHandler.hpp"
#include "Config.hpp"
#include "Hotbar.hpp"

Hotbar::Hotbar(Inventory &inventory, ClientCommandHandler &client, Widget *parent)
	: Widget(182, 22, parent), m_inventory(inventory), m_client(client)
{
	m_background.load("texture-widgets");
	m_background.setClipRect(0, 0, 182, 22);
	m_background.setPosition(0, 0, 0);

	m_cursor.load("texture-widgets");
	m_cursor.setClipRect(0, 22, 24, 24);
	m_cursor.setPosition(-1, -1, 0);
}

void Hotbar::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEWHEEL) {
		if (event.wheel.y < 0) {
			m_cursorPos = (m_cursorPos + 1) % 9;
			m_client.sendPlayerHeldItemChanged(m_cursorPos, currentItem().id());
		}
		else if (event.wheel.y > 0) {
			m_cursorPos = (m_cursorPos == 0) ? 8 : m_cursorPos - 1;
			m_client.sendPlayerHeldItemChanged(m_cursorPos, currentItem().id());
		}

		m_cursor.setPosition(-1 + 20 * m_cursorPos, -1, 0);
	}
}

void Hotbar::update() {
	for (u16 i = 0 ; i < 9 ; ++i) {
		if (m_items.size() <= i) {
			m_items.emplace_back(m_inventory, i, 0);

			ItemWidget &widget = m_items.back();
			widget.setPosition(5 + 20 * i - 3, 2, 0);
		}

		m_items[i].setStack(m_inventory.getStack(i, 0).item().stringID(), m_inventory.getStack(i, 0).amount());
	}
}

void Hotbar::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);

	for (auto &it : m_items)
		target.draw(it, states);

	target.draw(m_cursor, states);
}

