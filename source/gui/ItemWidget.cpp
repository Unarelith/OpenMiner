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
	m_id = id;

	m_image.load("texture-blocks");
	m_image.setClipRect(m_id * 16, 0, 16, 16);
	m_image.setScale(2.0f / 3.0f, 2.0f / 3.0f, 1.0f);
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	// FIXME: Image act weirdly when created in the constructor...
	target.draw(m_image, states);
}

