/*
 * =====================================================================================
 *
 *       Filename:  Hotbar.hpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 05:40:05
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef HOTBAR_HPP_
#define HOTBAR_HPP_

#include <vector>

#include "Inventory.hpp"
#include "ItemWidget.hpp"
#include "SDLHeaders.hpp"

class Hotbar : public Widget {
	public:
		Hotbar(Inventory &inventory, Widget *parent = nullptr);

		void onEvent(const S_Event &event);

		void update();

		int cursorPos() const { return m_cursorPos; }
		u16 currentItem() const { return m_inventory.getStack(m_cursorPos, 0).item().id(); }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Image m_cursor;
		int m_cursorPos = 0;

		Inventory &m_inventory;

		std::vector<ItemWidget> m_items;
};

#endif // HOTBAR_HPP_
