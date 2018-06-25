/*
 * =====================================================================================
 *
 *       Filename:  CraftingWidget.hpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 02:26:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CRAFTINGWIDGET_HPP_
#define CRAFTINGWIDGET_HPP_

#include "InventoryWidget.hpp"

class CraftingRecipe;

class CraftingWidget : public Widget {
	public:
		CraftingWidget(Widget *parent = nullptr);

		void onEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Inventory m_craftingInventory{3, 3};
		InventoryWidget m_craftingInventoryWidget{this};

		Inventory m_craftingResultInventory{1, 1};
		InventoryWidget m_craftingResultInventoryWidget{this};

		const CraftingRecipe *m_recipe = nullptr;
};

#endif // CRAFTINGWIDGET_HPP_
