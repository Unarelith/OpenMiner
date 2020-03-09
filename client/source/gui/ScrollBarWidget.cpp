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
#include <gk/core/Debug.hpp>

#include "Config.hpp"
#include "ScrollBarWidget.hpp"

void ScrollBarWidget::init(const std::string &texture, const gk::FloatRect &clipRect, InventoryWidget &widget) {
	m_clipRect = clipRect;

	m_widget = &widget;

	m_image.load(texture);
	m_image.setClipRect(m_clipRect.x, m_clipRect.y, m_clipRect.sizeX, m_clipRect.sizeY);
}

void ScrollBarWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		if (isPointInWidget(event.button.x, event.button.y)) {
			m_isDragging = true;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
		m_isDragging = false;
	}
	else if (event.type == SDL_MOUSEMOTION && m_isDragging) {
		s16 y = event.motion.y - m_minY * Config::guiScale - m_parent->getPosition().y;
		setPosition(getPosition().x, glm::clamp<s16>(y / Config::guiScale + m_height / 2, m_minY, m_maxY));

		m_scrolling = (getPosition().y - m_minY) / (m_maxY - m_minY);

		m_widget->scroll(m_scrolling);
	}
}

void ScrollBarWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_image, states);
}

