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

#include "client/graphics/Sprite.hpp"
#include "client/gui/ItemWidget.hpp"
#include "client/gui/Text.hpp"

class InventoryWidget;

class MouseItemWidget : public ItemWidget {
	public:
		MouseItemWidget(Widget *parent);

		void onEvent(const SDL_Event &event) override;

		void leftClickBehaviour();
		void rightClickBehaviour();

		void startDragging(bool isLeftClickDrag);
		void stopDragging();
		void draggingBehaviour(ItemWidget *newItemWidget);

		const ItemWidget *currentItemWidget() const { return m_currentItemWidget; }
		void updateCurrentItem(ItemWidget *currentItemWidget);

		void setCurrentInventoryWidget(InventoryWidget *inventoryWidget) { m_currentInventoryWidget = inventoryWidget; }

		const ItemStack &getStack() const { return m_inventory.getStack(0, 0); }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		void swapItems(ItemWidget &widget, bool isReadOnly = false);
		void putItem(ItemWidget &widget);

		void updatePosition(s32 x, s32 y);

		Inventory m_inventory{1, 1};

		ItemWidget *m_currentItemWidget = nullptr;
		InventoryWidget *m_currentInventoryWidget = nullptr;

		Sprite m_tooltipBackground{"texture-toasts", 160, 32};
		Text m_tooltipText;
		Text m_tooltipInfoText;

		bool m_isDragging = false;
		bool m_isLeftClickDrag = false;
		ItemWidget *m_dragOrigin = nullptr;
		std::unordered_map<ItemWidget *, ItemStack> m_draggedSlots;
		ItemStack m_draggedStack;
};

#endif // MOUSEITEMWIDGET_HPP_
