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

ItemWidget::ItemWidget(u16 id, Widget *parent) : Widget(16, 16, parent) {
	m_image.load("texture-blocks");
	m_image.setScale(2.0f / 3.0f, 2.0f / 3.0f, 1.0f);
	m_image.setPosition(3, 3, 0);

	setItem(id);
}

void ItemWidget::setItem(u16 id) {
	m_id = id;

	// if (id == 0)
	// 	id = 15;

	m_image.setClipRect(id * 16, 0, 16, 16);
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_image, states);
}

