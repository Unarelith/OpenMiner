/*
 * =====================================================================================
 *
 *       Filename:  LuaGUIState.hpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 00:47:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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

// FIXME: This class is almost a duplicate of InventoryState
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
