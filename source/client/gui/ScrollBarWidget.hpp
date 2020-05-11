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
#ifndef SCROLLBARWIDGET_HPP_
#define SCROLLBARWIDGET_HPP_

#include <gk/graphics/Image.hpp>

#include "InventoryWidget.hpp"

class ScrollBarWidget : public Widget {
	public:
		ScrollBarWidget(Widget *parent = nullptr) : Widget(12, 15, parent) {}

		void init(const sf::Texture &texture, const gk::FloatRect &clipRect, u16 minY, u16 maxY, InventoryWidget &widget);

		void onEvent(const sf::Event &event);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const;

		void updateScrolling(u16 y);

		gk::Image m_image;

		gk::FloatRect m_clipRect;

		u16 m_minY = 0;
		u16 m_maxY = 0;

		InventoryWidget *m_widget = nullptr;

		bool m_isDragging = false;

		float m_scrolling = 0;

		const u16 m_barWidth = 12;
		const u16 m_barHeight = 15;
};

#endif // SCROLLBARWIDGET_HPP_
