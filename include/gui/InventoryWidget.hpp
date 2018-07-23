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

#include "MouseItemWidget.hpp"
#include "RectangleShape.hpp"
#include "SDLHeaders.hpp"

class InventoryWidget : public Widget {
	public:
		InventoryWidget(Widget *parent = nullptr) : Widget(parent) {}

		void init(Inventory &inventory, unsigned int offset = 0, unsigned int size = 0);

		void onMouseEvent(const S_Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly = false);

		const ItemWidget *currentItemWidget() const { return m_currentItemWidget; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		u16 m_inventoryWidth = 0;
		u16 m_inventoryHeight = 0;

		std::vector<ItemWidget> m_itemWidgets;
		ItemWidget *m_currentItemWidget = nullptr;
		RectangleShape m_selectedItemBackground{16, 16, Color{255, 255, 255, 80}};
};

#endif // INVENTORYWIDGET_HPP_
