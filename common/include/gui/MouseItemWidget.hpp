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

#include <gk/graphics/Sprite.hpp>

#include "ItemWidget.hpp"
#include "Text.hpp"

class MouseItemWidget : public ItemWidget {
	public:
		MouseItemWidget(Widget *parent);

		void onEvent(const SDL_Event &event) override;

		void update(const ItemWidget *currentItemWidget);

		void swapItems(ItemWidget &widget, bool isReadOnly = false);
		void putItem(ItemWidget &widget);

		const ItemStack &getStack() const { return m_inventory.getStack(0, 0); }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void updatePosition(float x, float y);

		Inventory m_inventory{1, 1};

		const ItemWidget *m_currentItemWidget = nullptr;

		gk::Sprite m_tooltipBackground{"texture-toasts", 160, 32};
		Text m_tooltipText;
		Text m_tooltipInfoText;
};

#endif // MOUSEITEMWIDGET_HPP_
