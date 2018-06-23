/*
 * =====================================================================================
 *
 *       Filename:  ItemWidget.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:10:13
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMWIDGET_HPP_
#define ITEMWIDGET_HPP_

#include "Image.hpp"
#include "Inventory.hpp"
#include "Widget.hpp"

class ItemWidget : public Widget {
	public:
		ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent = nullptr);

		void update();

		u16 item() const { return m_inventory.getStack(m_x, m_y).item().id(); }
		void setItem(unsigned int id);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Inventory &m_inventory;

		unsigned int m_x = 0;
		unsigned int m_y = 0;

		Image m_image;
};

#endif // ITEMWIDGET_HPP_
