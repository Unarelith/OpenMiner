/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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

		void updateCurrentItem(const ItemWidget *currentItemWidget);

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
