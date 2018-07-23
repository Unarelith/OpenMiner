/*
 * =====================================================================================
 *
 *       Filename:  WorkbenchWidget.hpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:22:08
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WORKBENCHWIDGET_HPP_
#define WORKBENCHWIDGET_HPP_

#include "CraftingWidget.hpp"

class WorkbenchWidget : public Widget {
	public:
		WorkbenchWidget(Inventory &playerInventory, Inventory &craftingInventory, Widget *parent = nullptr);

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};
		InventoryWidget m_hotbarInventoryWidget{this};

		CraftingWidget m_craftingWidget;

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // WORKBENCHWIDGET_HPP_
