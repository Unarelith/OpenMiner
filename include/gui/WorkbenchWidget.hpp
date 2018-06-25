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
		WorkbenchWidget(Inventory &playerInventory, Inventory &hotbarInventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		CraftingWidget m_craftingWidget{this};

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};

		Inventory &m_hotbarInventory;
		InventoryWidget m_hotbarInventoryWidget{this};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // WORKBENCHWIDGET_HPP_
