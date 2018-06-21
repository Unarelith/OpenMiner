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

ItemWidget::ItemWidget(u16 id) {
	m_texture.load("textures/blocks.png");

	m_image.load(m_texture);
	m_image.setClipRect(id * 16, 0, 16, 16);
	m_image.setScale(2.0f / 3.0f, 2.0f / 3.0f, 1.0f);

	setPosition(10, 10, 0);
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_image, states);
}

