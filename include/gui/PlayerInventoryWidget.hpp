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
		PlayerInventoryWidget(Inventory &playerInventory, Widget *parent = nullptr);

		void onEvent(const sf::Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Inventory m_craftingInventory{2, 2};
		CraftingWidget m_craftingWidget{m_craftingInventory, this};

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};
		InventoryWidget m_hotbarInventoryWidget{this};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // PLAYERINVENTORYWIDGET_HPP_
