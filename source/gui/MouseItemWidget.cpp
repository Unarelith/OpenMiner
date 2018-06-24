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
	if (event.type == SDL_MOUSEMOTION && stack().item().id() != 0) {
		updatePosition(event.motion.x, event.motion.y);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		updatePosition(event.button.x, event.button.y);
	}
}

void MouseItemWidget::swapItems(ItemWidget &widget) {
	u32 id = widget.stack().item().id();
	u32 amount = widget.stack().amount();
	if (stack().item().id() != id) {
		widget.setStack(stack().item().id(), stack().amount());
		setStack(id, amount);
	}
	else {
		widget.setStack(widget.stack().item().id(), widget.stack().amount() + stack().amount());
		setStack(0, 0);
	}
}

void MouseItemWidget::putItem(ItemWidget &widget) {
	if (!widget.stack().item().id()) {
		widget.setStack(stack().item().id(), 1);
		setStack(stack().item().id(), stack().amount() - 1);
	}
	else if (widget.stack().item().id() == stack().item().id()) {
		widget.setStack(stack().item().id(), widget.stack().amount() + 1);
		setStack(stack().item().id(), stack().amount() - 1);
	}
	else if (stack().item().id() == 0) {
		setStack(widget.stack().item().id(), ceil(widget.stack().amount() / 2.0));
		widget.setStack(widget.stack().item().id(), widget.stack().amount() / 2);
	}
}

void MouseItemWidget::updatePosition(float x, float y) {
	x = x / 3 - m_parent->getPosition().x - 8;
	y = y / 3 - m_parent->getPosition().y - 8;

	setPosition(x, y, 0);
}

