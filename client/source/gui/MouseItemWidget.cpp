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
#include "MouseItemWidget.hpp"

MouseItemWidget::MouseItemWidget(Widget *parent) : ItemWidget(m_inventory, 0, 0, parent) {
	m_tooltipBackground.setColor(gk::Color{255, 255, 255, 240});
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

void MouseItemWidget::updateCurrentItem(const ItemWidget *currentItemWidget) {
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
	std::string widgetItemName = widget.stack().item().stringID();
	u32 widgetItemAmount = widget.stack().amount();

	if (!isReadOnly || stack().item().id() == 0 || stack().item().stringID() == widgetItemName) {
		if (stack().item().stringID() != widgetItemName) {
			widget.setStack(stack().item().stringID(), stack().amount());
			setStack(widgetItemName, widgetItemAmount);
		}
		else if (!isReadOnly) {
			widget.setStack(widgetItemName, widgetItemAmount + stack().amount());
			setStack("", 0);
		}
		else {
			setStack(stack().item().stringID(), stack().amount() + widgetItemAmount);
			widget.setStack("", 0);
		}
	}
}

void MouseItemWidget::putItem(ItemWidget &widget) {
	std::string widgetItemName = widget.stack().item().stringID();
	u32 widgetItemID = widget.stack().item().id();
	u32 widgetItemAmount = widget.stack().amount();

	if (!widgetItemID && stack().item().id()) {
		widget.setStack(stack().item().stringID(), 1);
		setStack(stack().amount() > 1 ? stack().item().stringID() : "", stack().amount() - 1);
	}
	else if (widgetItemID && widgetItemID == stack().item().id()) {
		widget.setStack(stack().item().stringID(), widgetItemAmount + 1);
		setStack(stack().amount() > 1 ? stack().item().stringID() : "", stack().amount() - 1);
	}
	else if (stack().item().id() == 0) {
		setStack(widgetItemName, ceil(widgetItemAmount / 2.0));
		widget.setStack(widgetItemAmount > 1 ? widgetItemName : "", widgetItemAmount / 2);
	}
}

void MouseItemWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
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

