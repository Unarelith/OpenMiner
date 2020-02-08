/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "Widget.hpp"

bool Widget::isPointInWidget(float x, float y) {
	return getGlobalBounds().intersects(gk::FloatRect{x, y, 1, 1});
}

gk::FloatRect Widget::getGlobalBounds() const {
	gk::FloatRect aabb{0, 0, static_cast<float>(m_width), static_cast<float>(m_height)};

	const Widget *widget = this;
	while (widget) {
		aabb.x *= widget->getScale().x;
		aabb.y *= widget->getScale().y;

		aabb.x += widget->getPosition().x;
		aabb.y += widget->getPosition().y;

		aabb.width *= widget->getScale().x;
		aabb.height *= widget->getScale().y;

		widget = widget->m_parent;
	}

	return aabb;
}

