/*
 * =====================================================================================
 *
 *       Filename:  MouseItemWidget.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 00:50:38
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef MOUSEITEMWIDGET_HPP_
#define MOUSEITEMWIDGET_HPP_

#include "ItemWidget.hpp"

class MouseItemWidget : public ItemWidget {
	public:
		MouseItemWidget(Widget *parent) : ItemWidget(m_inventory, 0, 0, parent) { update(); }

		void onEvent(const SDL_Event &event);

		void swapItems(ItemWidget &widget);

	private:
		void updatePosition(float x, float y);

		Inventory m_inventory{1, 1};
};

#endif // MOUSEITEMWIDGET_HPP_
