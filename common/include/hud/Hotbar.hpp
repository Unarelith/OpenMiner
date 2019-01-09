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

class Hotbar : public Widget {
	public:
		Hotbar(Inventory &inventory, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		int cursorPos() const { return m_cursorPos; }
		u16 currentItem() const { return m_inventory.getStack(m_cursorPos, 0).item().id(); }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Image m_background;

		gk::Image m_cursor;
		int m_cursorPos = 0;

		Inventory &m_inventory;

		std::vector<ItemWidget> m_items;
};

#endif // HOTBAR_HPP_
