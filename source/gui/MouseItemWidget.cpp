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
	m_tooltipBackground.setColor(Color{255, 255, 255, 240});
	m_tooltipBackground.setPosition(20, 17, 0);

	m_tooltipText.setPosition(26, 24, 0);

	m_tooltipInfoText.setPosition(26, 35, 0);
	m_tooltipInfoText.setColor({180, 180, 180});
}

void MouseItemWidget::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		updatePosition(event.motion.x, event.motion.y);
	}

	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		updatePosition(event.button.x, event.button.y);
	}
}

void MouseItemWidget::update(const ItemWidget *currentItemWidget) {
	if (currentItemWidget) {
		m_currentItemWidget = (currentItemWidget->stack().item().id()) ? currentItemWidget : nullptr;
		m_tooltipText.setText(currentItemWidget->stack().item().label() + " [" + std::to_string(currentItemWidget->stack().item().id()) + "]");

		if (currentItemWidget->stack().item().isFuel())
			m_tooltipInfoText.setText("Burn time: " + std::to_string(currentItemWidget->stack().item().burnTime()) + " ticks");
		else
			m_tooltipInfoText.setText("");
	}
	else {
		m_currentItemWidget = nullptr;
	}
}

void MouseItemWidget::swapItems(ItemWidget &widget, bool isReadOnly) {
	u32 widgetItemID = widget.stack().item().id();
	u32 widgetItemAmount = widget.stack().amount();

	if (!isReadOnly || stack().item().id() == 0 || stack().item().id() == widgetItemID) {
		if (stack().item().id() != widgetItemID) {
			widget.setStack(stack().item().id(), stack().amount());
			setStack(widgetItemID, widgetItemAmount);
		}
		else if (!isReadOnly) {
			widget.setStack(widgetItemID, widgetItemAmount + stack().amount());
			setStack(0, 0);
		}
		else {
			setStack(stack().item().id(), stack().amount() + widgetItemAmount);
			widget.setStack(0, 0);
		}
	}
}

void MouseItemWidget::putItem(ItemWidget &widget) {
	u32 widgetItemID = widget.stack().item().id();
	u32 widgetItemAmount = widget.stack().amount();

	if (!widgetItemID && stack().item().id()) {
		widget.setStack(stack().item().id(), 1);
		setStack(stack().amount() > 1 ? stack().item().id() : 0, stack().amount() - 1);
	}
	else if (widgetItemID && widgetItemID == stack().item().id()) {
		widget.setStack(stack().item().id(), widgetItemAmount + 1);
		setStack(stack().amount() > 1 ? stack().item().id() : 0, stack().amount() - 1);
	}
	else if (stack().item().id() == 0) {
		setStack(widgetItemID, ceil(widgetItemAmount / 2.0));
		widget.setStack(widgetItemAmount > 1 ? widgetItemID : 0, widgetItemAmount / 2);
	}
}

void MouseItemWidget::draw(RenderTarget &target, RenderStates states) const {
	ItemWidget::draw(target, states);

	states.transform *= getTransform();

	if (m_currentItemWidget) {
		target.draw(m_tooltipBackground, states);
		target.draw(m_tooltipText, states);
		target.draw(m_tooltipInfoText, states);
	}
}

void MouseItemWidget::updatePosition(float x, float y) {
	x -= m_parent->getPosition().x + 10 * m_parent->getScale().x;
	y -= m_parent->getPosition().y + 10 * m_parent->getScale().y;

	setPosition(x / m_parent->getScale().x, y / m_parent->getScale().y, 0);
}

