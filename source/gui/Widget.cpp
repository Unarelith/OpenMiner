/*
 * =====================================================================================
 *
 *       Filename:  Widget.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 07:55:39
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Widget.hpp"

bool Widget::isPointInWidget(float x, float y) {
	return getGlobalBounds().intersects(FloatRect{x, y, 1, 1});
}

FloatRect Widget::getGlobalBounds() const {
	FloatRect aabb{0, 0, static_cast<float>(m_width), static_cast<float>(m_height)};

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

