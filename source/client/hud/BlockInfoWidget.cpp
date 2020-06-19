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

void BlockInfoWidget::setCurrentBlock(const Block *block) {
	m_currentBlock = block;

	if (!m_currentBlock)
		m_isVisible = false;
	else {
		m_isVisible = true;

		m_text.setString(block->label());
		m_itemWidget.setStack(block->stringID(), 1);
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

