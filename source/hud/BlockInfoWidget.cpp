/*
 * =====================================================================================
 *
 *       Filename:  BlockInfoWidget.cpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 14:34:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Block.hpp"
#include "BlockInfoWidget.hpp"

BlockInfoWidget::BlockInfoWidget(Widget *parent) : Widget(160, 32, parent) {
	m_itemWidget.setPosition(5, m_height / 2 - m_itemWidget.height() / 2, 0);

	m_background.setColor(Color{255, 255, 255, 200});

	m_text.setColor(Color{240, 240, 240});
	m_text.setPosition(26, 8, 0);
}

void BlockInfoWidget::update() {
	m_itemWidget.update();
}

void BlockInfoWidget::setCurrentBlock(const Block *block) {
	m_currentBlock = block;

	if (!m_currentBlock)
		m_isVisible = false;
	else {
		m_isVisible = true;

		m_text.setText(block->label());
		m_itemWidget.setStack(block->id(), 1);
	}
}

void BlockInfoWidget::draw(RenderTarget &target, RenderStates states) const {
	if (m_isVisible) {
		states.transform *= getTransform();

		target.draw(m_background, states);
		target.draw(m_itemWidget, states);
		target.draw(m_text, states);
	}
}

