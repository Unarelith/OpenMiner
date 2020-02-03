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

class Recipe;

class CraftingWidget : public Widget {
	public:
		CraftingWidget(ClientCommandHandler &client, Inventory &craftingInventory, Widget *parent = nullptr);

		void init(unsigned int offset = 0, unsigned int size = 3);

		void onMouseEvent(const SDL_Event &event, MouseItemWidget &mouseItemWidget);

		void update() override;

		const ItemWidget *currentItemWidget() const { return m_craftingResultInventoryWidget.currentItemWidget() ? m_craftingResultInventoryWidget.currentItemWidget() : m_craftingInventoryWidget.currentItemWidget(); }

		InventoryWidget &craftingInventoryWidget() { return m_craftingInventoryWidget; }
		InventoryWidget &craftingResultInventoryWidget() { return m_craftingResultInventoryWidget; }

	protected:
		ClientCommandHandler &m_client;

		Inventory &m_craftingInventory;
		InventoryWidget m_craftingInventoryWidget{m_client, this};

		Inventory m_craftingResultInventory{1, 1};
		InventoryWidget m_craftingResultInventoryWidget{m_client, this};

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		const Recipe *m_recipe = nullptr;
};

#endif // CRAFTINGWIDGET_HPP_
