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

		void loadGUI(sf::Packet &packet);
		void loadImage(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadTextButton(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadInventoryWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadCraftingWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadProgressBarWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadScrollBarWidget(const std::string &name, s32 x, s32 y, sf::Packet &packet);
		void loadInventory(const std::string &name, sf::Packet &packet);

		gk::Texture &loadTexture(const std::string &textureFilename);

		void centerMainWidget();

		ClientCommandHandler &m_client;

		Widget m_mainWidget;

		MouseItemWidget m_mouseItemWidget{&m_mainWidget};
		InventoryWidget *m_currentInventoryWidget = nullptr;

		std::unordered_map<std::string, CraftingWidget> m_craftingWidgets;
		std::unordered_map<std::string, InventoryWidget> m_inventoryWidgets;
		std::vector<std::unique_ptr<Widget>> m_widgets;
		std::vector<std::unique_ptr<gk::Drawable>> m_drawables;
		std::unordered_map<std::string, Inventory> m_inventories;
		std::unordered_map<std::string, gk::Texture> m_textures;

		ClientPlayer &m_player;
		ClientWorld &m_world;

		u16 m_width = 0;
		u16 m_height = 0;

		bool m_isCentered = false;
};

#endif // LUAGUISTATE_HPP_
