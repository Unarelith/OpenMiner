/*
 * =====================================================================================
 *
 *       Filename:  ItemWidget.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:11:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemWidget.hpp"

ItemWidget::ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent)
	: Widget(16, 16, parent), m_inventory(inventory), m_x(x), m_y(y)
{
	m_image.load("texture-blocks");
	m_image.setScale(2.0f / 3.0f, 2.0f / 3.0f, 1.0f);
	m_image.setPosition(3, 3, 0);
	m_image.setClipRect(0, 0, 0, 0);
}

void ItemWidget::update() {
	m_image.setClipRect(item() * 16, item() / 16 * 16, 16, 16);
}

void ItemWidget::setItem(unsigned int id) {
	m_inventory.setStack(m_x, m_y, id);
	update();
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_image, states);
}

