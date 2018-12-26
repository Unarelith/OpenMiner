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
	return getGlobalBounds().intersects(sf::FloatRect{x, y, 1, 1});
}

sf::FloatRect Widget::getGlobalBounds() const {
	sf::FloatRect aabb{0, 0, static_cast<float>(m_width), static_cast<float>(m_height)};

	const Widget *widget = this;
	while (widget) {
		aabb.left *= widget->getScale().x;
		aabb.top *= widget->getScale().y;

		aabb.left += widget->getPosition().x;
		aabb.top += widget->getPosition().y;

		aabb.width *= widget->getScale().x;
		aabb.height *= widget->getScale().y;

		widget = widget->m_parent;
	}

	return aabb;
}

