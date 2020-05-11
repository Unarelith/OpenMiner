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

void ScrollBarWidget::init(const sf::Texture &texture, const gk::FloatRect &clipRect, u16 minY, u16 maxY, InventoryWidget &widget) {
	m_clipRect = clipRect;

	m_minY = minY;
	m_maxY = maxY;

	m_width = m_barWidth;
	m_height = m_maxY - m_minY + m_barHeight;

	m_widget = &widget;

	m_image.load(texture);
	m_image.setClipRect(m_clipRect.x, m_clipRect.y, m_clipRect.sizeX, m_clipRect.sizeY);
}

void ScrollBarWidget::onEvent(const sf::Event &event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (isPointInWidget(event.mouseButton.x, event.mouseButton.y)) {
			m_isDragging = true;

			updateScrolling(event.mouseButton.y);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		m_isDragging = false;
	}
	else if (event.type == sf::Event::MouseMoved && m_isDragging) {
		updateScrolling(event.mouseMove.y);
	}
}

void ScrollBarWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_image, states);
}

void ScrollBarWidget::updateScrolling(u16 y) {
	s16 imageY = y - getPosition().y * Config::guiScale - m_parent->getPosition().y;
	m_image.setPosition(0, glm::clamp<s16>(imageY / Config::guiScale - m_barHeight / 2, m_minY, m_maxY));

	m_scrolling = m_image.getPosition().y / (m_maxY - m_minY);

	m_widget->scroll(m_scrolling);
}

