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
		CraftingWidget(Inventory &craftingInventory, Widget *parent = nullptr);

		void onMouseEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget);

		void update() override;

		const ItemWidget *currentItemWidget() const { return m_craftingResultInventoryWidget.currentItemWidget() ? m_craftingResultInventoryWidget.currentItemWidget() : m_craftingInventoryWidget.currentItemWidget(); }

	protected:
		Inventory &m_craftingInventory;
		InventoryWidget m_craftingInventoryWidget{this};

		Inventory m_craftingResultInventory{1, 1};
		InventoryWidget m_craftingResultInventoryWidget{this};

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		const CraftingRecipe *m_recipe = nullptr;
};

#endif // CRAFTINGWIDGET_HPP_
