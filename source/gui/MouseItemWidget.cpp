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

MouseItemWidget::MouseItemWidget(Widget *parent) : ItemWidget(m_inventory, 0, 0, parent) {
	m_tooltipBackground.setPosition(20, 20, 0);
	m_tooltipText.setPosition(27, 27, 0);
}

void MouseItemWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		updatePosition(event.motion.x, event.motion.y);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		updatePosition(event.button.x, event.button.y);
	}
}

void MouseItemWidget::update(const ItemWidget *currentItemWidget) {
	if (currentItemWidget) {
		m_currentItemWidget = (currentItemWidget->stack().item().id()) ? currentItemWidget : nullptr;
		m_tooltipText.setText(currentItemWidget->stack().item().name() + " [" + std::to_string(currentItemWidget->stack().item().id()) + "]");
	}
	else {
		m_currentItemWidget = nullptr;
	}
}

// FIXME
void MouseItemWidget::swapItems(ItemWidget &widget, bool isReadOnly) {
	u32 id = widget.stack().item().id();
	u32 amount = widget.stack().amount();
	if (!isReadOnly || stack().item().id() == 0 || stack().item().id() == id) {
		if (stack().item().id() != id) {
			widget.setStack(stack().item().id(), stack().amount());
			setStack(id, amount);
		}
		else if (!isReadOnly) {
			widget.setStack(widget.stack().item().id(), widget.stack().amount() + stack().amount());
			setStack(0, 0);
		}
		else {
			setStack(stack().item().id(), stack().amount() + widget.stack().amount());
			widget.setStack(0, 0);
		}
	}
}

// FIXME
void MouseItemWidget::putItem(ItemWidget &widget) {
	if (!widget.stack().item().id()) {
		widget.setStack(stack().item().id(), 1);
		setStack(stack().amount() > 1 ? stack().item().id() : 0, stack().amount() - 1);
	}
	else if (widget.stack().item().id() == stack().item().id()) {
		widget.setStack(stack().item().id(), widget.stack().amount() + 1);
		setStack(stack().amount() > 1 ? stack().item().id() : 0, stack().amount() - 1);
	}
	else if (stack().item().id() == 0) {
		setStack(widget.stack().item().id(), ceil(widget.stack().amount() / 2.0));
		widget.setStack(widget.stack().amount() > 1 ? widget.stack().item().id() : 0, widget.stack().amount() / 2);
	}
}

void MouseItemWidget::draw(RenderTarget &target, RenderStates states) const {
	ItemWidget::draw(target, states);

	applyTransform(states);

	if (m_currentItemWidget) {
		target.draw(m_tooltipBackground, states);
		target.draw(m_tooltipText, states);
	}
}

void MouseItemWidget::updatePosition(float x, float y) {
	x -= m_parent->getPosition().x + 10 * m_parent->getScale().x;
	y -= m_parent->getPosition().y + 10 * m_parent->getScale().y;

	setPosition(x / m_parent->getScale().x, y / m_parent->getScale().y, 0);
}

