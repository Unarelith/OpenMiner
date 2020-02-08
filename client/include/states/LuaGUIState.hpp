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
#ifndef LUAGUISTATE_HPP_
#define LUAGUISTATE_HPP_

#include <deque>
#include <memory>

#include <gk/graphics/RectangleShape.hpp>

#include "CraftingWidget.hpp"
#include "InterfaceState.hpp"
#include "InventoryWidget.hpp"
#include "MouseItemWidget.hpp"

class ClientPlayer;
class ClientWorld;

class LuaGUIState : public InterfaceState {
	public:
		LuaGUIState(ClientCommandHandler &client, ClientPlayer &player, ClientWorld &world, sf::Packet &packet, gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void loadGUI(ClientPlayer &player, ClientWorld &world, sf::Packet &packet);

		ClientCommandHandler &m_client;

		Widget m_mainWidget;

		MouseItemWidget m_mouseItemWidget{&m_mainWidget};

		std::deque<CraftingWidget> m_craftingWidgets;
		std::deque<InventoryWidget> m_inventoryWidgets;
		std::vector<std::unique_ptr<Widget>> m_widgets;
		std::vector<std::unique_ptr<gk::Drawable>> m_drawables;
};

#endif // LUAGUISTATE_HPP_
