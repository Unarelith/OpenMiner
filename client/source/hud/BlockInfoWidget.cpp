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

	m_background.setColor(gk::Color{255, 255, 255, 200});

	m_text.setColor(gk::Color{240, 240, 240});
	m_text.setPosition(26, 8, 0);
}

void BlockInfoWidget::update() {
	m_itemWidget.update();
}

void BlockInfoWidget::setCurrentBlock(const Block *block, const glm::vec4 &selectedBlock) {
	m_currentBlock = block;
	m_selectedBlock = selectedBlock;

	if (!m_currentBlock)
		m_isVisible = false;
	else {
		m_isVisible = true;

		m_text.setText(block->label() + "\n"
				+ std::to_string((int)selectedBlock.x) + ", "
				+ std::to_string((int)selectedBlock.y) + ", "
				+ std::to_string((int)selectedBlock.z));
		m_itemWidget.setStack(block->name(), 1);
	}
}

void BlockInfoWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_isVisible) {
		states.transform *= getTransform();

		target.draw(m_background, states);
		target.draw(m_itemWidget, states);
		target.draw(m_text, states);
	}
}

