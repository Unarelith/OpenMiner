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
#include "common/core/Debug.hpp"

#include "client/core/Config.hpp"
#include "client/gui/ScrollBarWidget.hpp"

void ScrollBarWidget::init(const Texture &texture, const FloatRect &clipRect, u16 minY, u16 maxY, InventoryWidget &widget) {
	m_clipRect = clipRect;

	m_minY = minY;
	m_maxY = maxY;

	m_width = m_barWidth;
	m_height = m_maxY - m_minY + m_barHeight;

	m_widget = &widget;

	m_image.load(texture);
	m_image.setClipRect(m_clipRect.x, m_clipRect.y, (u16)m_clipRect.sizeX, (u16)m_clipRect.sizeY);
}

void ScrollBarWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		if (isPointInWidget(event.button.x, event.button.y)) {
			m_isDragging = true;

			updateScrolling((u16)event.button.y);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
		m_isDragging = false;
	}
	else if (event.type == SDL_MOUSEMOTION && m_isDragging) {
		updateScrolling((u16)event.motion.y);
	}
}

void ScrollBarWidget::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_image, states);
}

void ScrollBarWidget::updateScrolling(u16 y) {
	float imageY = floorf(y - getPosition().y * Config::guiScale - m_parent->getPosition().y);
	m_image.setPosition(0, glm::clamp<float>(imageY / Config::guiScale - m_barHeight / 2, (float)m_minY, (float)m_maxY));

	m_scrolling = m_image.getPosition().y / (m_maxY - m_minY);

	m_widget->scroll(m_scrolling);
}
