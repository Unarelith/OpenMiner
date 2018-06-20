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
	m_image.setPosRect(10, 10, 16 * 2, 16 * 2);
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_image, states);
}

