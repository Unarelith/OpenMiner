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
	: Widget(18, 18, parent), m_inventory(inventory), m_x(x), m_y(y)
{
}

void ItemWidget::update() {
	if (stack().item().isBlock()) {
		m_image.load("texture-blocks");
		m_image.setClipRect(stack().item().textureID() * 16, stack().item().textureID() / 16 * 16, 16, 16);
		m_image.setScale(2.0f / 3.0f, 2.0f / 3.0f, 1.0f);
		m_image.setPosition(3, 3, 0);
	}
	else {
		m_image.load("texture-items");
		m_image.setClipRect(stack().item().textureID() * 16, stack().item().textureID() / 16 * 16, 16, 16);
		m_image.setScale(1, 1, 1);
		m_image.setPosition(0.6, 0.6, 0);
	}

	m_text.setText(std::to_string(stack().amount()));
	m_text.setPosition(16 - 4 - 6 * floor(log10(stack().amount())), 16 - 6, 0);
}

void ItemWidget::setStack(unsigned int id, unsigned int amount) {
	m_inventory.setStack(m_x, m_y, id, amount);
	update();
}

void ItemWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_image, states);

	if (stack().item().id() && stack().amount() > 1)
		target.draw(m_text, states);
}

