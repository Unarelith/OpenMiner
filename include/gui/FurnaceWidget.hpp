/*
 * =====================================================================================
 *
 *       Filename:  FurnaceWidget.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:02:21
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef FURNACEWIDGET_HPP_
#define FURNACEWIDGET_HPP_

#include "InventoryWidget.hpp"

class FurnaceWidget : public Widget {
	public:
		FurnaceWidget(Inventory &playerInventory, Inventory &hotbarInventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};

		Inventory &m_hotbarInventory;
		InventoryWidget m_hotbarInventoryWidget{this};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // FURNACEWIDGET_HPP_
