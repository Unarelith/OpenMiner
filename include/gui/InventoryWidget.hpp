/*
 * =====================================================================================
 *
 *       Filename:  InventoryWidget.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:08:40
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INVENTORYWIDGET_HPP_
#define INVENTORYWIDGET_HPP_

#include "Inventory.hpp"
#include "ItemWidget.hpp"
#include "SDLHeaders.hpp"

class InventoryWidget : public Widget {
	public:
		InventoryWidget(Widget *parent = nullptr) : Widget(parent) {}

		void onEvent(const SDL_Event &event);

		void update(const Inventory &inventory);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		std::vector<ItemWidget> m_itemWidgets;

		int m_selectedItem = -1;
};

#endif // INVENTORYWIDGET_HPP_
