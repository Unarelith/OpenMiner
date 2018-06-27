/*
 * =====================================================================================
 *
 *       Filename:  SmallCraftingWidget.cpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 09:04:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "SmallCraftingWidget.hpp"

SmallCraftingWidget::SmallCraftingWidget(Widget *parent) : CraftingWidget(2, 2, parent) {
	m_craftingInventoryWidget.setPosition(97, 17, 0);
	m_craftingResultInventoryWidget.setPosition(153, 27, 0);
}

