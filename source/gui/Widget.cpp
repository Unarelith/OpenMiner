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
#include "Rect.hpp"
#include "Widget.hpp"

#include "Debug.hpp"

bool Widget::isPointInWidget(float x, float y) {
	FloatRect aabb;
	aabb.x = getPosition().x;
	aabb.y = getPosition().y;
	aabb.width = m_width * getScale().x;
	aabb.height = m_height * getScale().y;

	Widget *parent = m_parent;
	while (parent) {
		aabb.x += parent->getPosition().x;
		aabb.y += parent->getPosition().y;

		aabb.width *= parent->getScale().x;
		aabb.height *= parent->getScale().y;

		parent = parent->m_parent;
	}

	// DEBUG("Checking aabb:", aabb.x, aabb.y, aabb.width, aabb.height, "|", x, y);

	return aabb.intersects(FloatRect{x, y, 1, 1});
}

