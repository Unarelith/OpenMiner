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

#include "SmallCraftingWidget.hpp"

class PlayerInventoryWidget : public Widget {
	public:
		PlayerInventoryWidget(Inventory &playerInventory, Inventory &hotbarInventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		SmallCraftingWidget m_craftingWidget{this};

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};

		Inventory &m_hotbarInventory;
		InventoryWidget m_hotbarInventoryWidget{this};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // PLAYERINVENTORYWIDGET_HPP_
