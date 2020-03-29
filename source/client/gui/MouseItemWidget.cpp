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
#include "InventoryWidget.hpp"
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

		if (getStack().amount() == 0) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				leftClickBehaviour();
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				rightClickBehaviour();
			}

			m_isDragging = false;
		}
		else if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT) {
			startDragging(event.button.button == SDL_BUTTON_LEFT);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		updatePosition(event.button.x, event.button.y);

		if (m_isDragging && m_draggedSlots.size() <= 1) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				leftClickBehaviour();
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
				rightClickBehaviour();
			}
		}

		stopDragging();
	}
}

void MouseItemWidget::leftClickBehaviour() {
	if (m_currentInventoryWidget && m_currentInventoryWidget->currentItemWidget() && m_currentInventoryWidget->inventory()) {
		ItemWidget *currentItemWidget = m_currentInventoryWidget->currentItemWidget();
		if (!m_currentInventoryWidget->inventory()->isUnlimited())
			swapItems(*currentItemWidget, m_currentInventoryWidget->isReadOnly());
		else if (getStack().amount() == 0 && currentItemWidget->stack().amount() != 0)
			setStack(currentItemWidget->stack().item().stringID(), 64);

		m_currentInventoryWidget->sendUpdatePacket();
	}
}

void MouseItemWidget::rightClickBehaviour() {
	if (m_currentInventoryWidget && m_currentInventoryWidget->currentItemWidget() && m_currentInventoryWidget->inventory()) {
		if (!m_currentInventoryWidget->isReadOnly()) {
			ItemWidget *currentItemWidget = m_currentInventoryWidget->currentItemWidget();
			if (!m_currentInventoryWidget->inventory()->isUnlimited())
				putItem(*currentItemWidget);
			else if (getStack().amount() == 0 && currentItemWidget->stack().amount() != 0)
				setStack(currentItemWidget->stack().item().stringID(), 1);

			m_currentInventoryWidget->sendUpdatePacket();
		}
	}
}

void MouseItemWidget::startDragging(bool isLeftClickDrag) {
	m_isDragging = true;
	m_isLeftClickDrag = isLeftClickDrag;
	m_draggedStack = getStack();
	m_dragOrigin = nullptr;

	if (m_currentItemWidget && (m_currentItemWidget->stack().amount() == 0 || m_currentItemWidget->stack().item().stringID() == m_draggedStack.item().stringID())) {
		m_dragOrigin = m_currentItemWidget;
		m_draggedSlots[m_currentItemWidget] = m_currentItemWidget->stack();
	}
}

void MouseItemWidget::stopDragging() {
	m_isDragging = false;

	for (auto &it : m_draggedSlots)
		it.first->setChanged(true);

	m_draggedSlots.clear();
}

void MouseItemWidget::draggingBehaviour(ItemWidget *newItemWidget) {
	if (newItemWidget->stack().amount() == 0 || newItemWidget->stack().item().stringID() == m_draggedStack.item().stringID()) {
		auto it = m_draggedSlots.find(newItemWidget);
		if (it == m_draggedSlots.end()) {
			if (m_draggedSlots.empty())
				m_dragOrigin = newItemWidget;

			m_draggedSlots.emplace(std::make_pair(newItemWidget, newItemWidget->stack()));
		}
	}

	if (m_draggedSlots.size() > 1) {
		if (m_isLeftClickDrag) {
			for (auto &it : m_draggedSlots) {
				u16 splitAmount = m_draggedStack.amount() / m_draggedSlots.size();

				if (splitAmount > 0) {
					it.first->setStack(m_draggedStack.item().stringID(), it.second.amount() + splitAmount);
					it.first->update();
				}
			}

			u16 remainingAmount = (m_draggedStack.amount() == 1) ? 0 : m_draggedStack.amount() % m_draggedSlots.size();

			setStack(remainingAmount ? m_draggedStack.item().stringID() : "_:air", remainingAmount);
		}
		else if (m_draggedStack.amount() > 0 && (newItemWidget->stack().amount() == 0 || newItemWidget->stack().item().stringID() == m_draggedStack.item().stringID())) {
			if (m_dragOrigin) {
				m_dragOrigin->setStack(m_draggedStack.item().stringID(), m_dragOrigin->stack().amount() + 1);

				m_draggedStack.setAmount(m_draggedStack.amount() - 1);
				setStack(m_draggedStack.amount() ? m_draggedStack.item().stringID() : "_:air", m_draggedStack.amount());

				m_dragOrigin = nullptr;
			}

			newItemWidget->setStack(m_draggedStack.item().stringID(), newItemWidget->stack().amount() + 1);

			m_draggedStack.setAmount(m_draggedStack.amount() - 1);
			setStack(m_draggedStack.amount() ? m_draggedStack.item().stringID() : "_:air", m_draggedStack.amount());
		}
	}
}

void MouseItemWidget::updateCurrentItem(ItemWidget *currentItemWidget) {
	if (currentItemWidget) {
		if (m_isDragging && currentItemWidget != m_currentItemWidget)
			draggingBehaviour(currentItemWidget);

		m_currentItemWidget = (currentItemWidget->stack().item().id()) ? currentItemWidget : nullptr;
		m_tooltipText.setText(currentItemWidget->stack().item().label() + " [" + std::to_string(currentItemWidget->stack().item().id()) + "]");

		if (currentItemWidget->stack().item().hasGroup("om_fuel"))
			m_tooltipInfoText.setText("Burn time: " + std::to_string(currentItemWidget->stack().item().getGroupValue("om_fuel")) + " ticks");
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
	if (getStack().amount() > 0)
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

	setPosition(x / m_parent->getScale().x, y / m_parent->getScale().y, -20);
}

