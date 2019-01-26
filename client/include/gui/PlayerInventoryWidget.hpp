/*
 * =====================================================================================
 *
 *       Filename:  PlayerInventoryWidget.hpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 05:32:49
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PLAYERINVENTORYWIDGET_HPP_
#define PLAYERINVENTORYWIDGET_HPP_

#include "CraftingWidget.hpp"

class PlayerInventoryWidget : public Widget {
	public:
		PlayerInventoryWidget(Client &client, Inventory &playerInventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Client &m_client;

		gk::Image m_background;

		Inventory m_craftingInventory{2, 2};
		CraftingWidget m_craftingWidget{m_client, m_craftingInventory, this};

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{m_client, this};
		InventoryWidget m_hotbarInventoryWidget{m_client, this};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // PLAYERINVENTORYWIDGET_HPP_
