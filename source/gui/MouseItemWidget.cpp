/*
 * =====================================================================================
 *
 *       Filename:  MouseItemWidget.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 00:51:15
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "MouseItemWidget.hpp"

void MouseItemWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION && item() != 0) {
		updatePosition(event.motion.x, event.motion.y);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		updatePosition(event.button.x, event.button.y);
	}
}

void MouseItemWidget::swapItems(ItemWidget &widget) {
	u16 widgetItem = widget.item();
	widget.setItem(item());
	setItem(widgetItem);
}

void MouseItemWidget::updatePosition(float x, float y) {
	x = x / 3 - m_parent->getPosition().x - 8;
	y = y / 3 - m_parent->getPosition().y - 8;

	setPosition(x, y, 0);
}

