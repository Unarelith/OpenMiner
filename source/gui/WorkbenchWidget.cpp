/*
 * =====================================================================================
 *
 *       Filename:  WorkbenchWidget.cpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:22:29
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "WorkbenchWidget.hpp"

WorkbenchWidget::WorkbenchWidget() {
	m_backgroundTexture.load("textures/workbench.png");
	m_background.load(m_backgroundTexture);
	m_background.setClipRect(0, 0, 176, 166);
	m_background.setPosRect(SCREEN_WIDTH  / 2.0 - m_background.clipRect().width  * 3.0 / 2.0,
	                        SCREEN_HEIGHT / 2.0 - m_background.clipRect().height * 3.0 / 2.0,
	                        m_background.clipRect().width * 3,
	                        m_background.clipRect().height * 3);

	for (u16 i = 1 ; i < 10 ; ++i)
		m_inventory.addItem(i);

	m_inventoryWidget.update(m_inventory);
}

void WorkbenchWidget::onEvent(const SDL_Event &event) {
}

void WorkbenchWidget::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_background, states);
	target.draw(m_inventoryWidget, states);

		// Image image;
		// image.load(m_blocksTexture);
		// image.setClipRect(i * 16, 0, 16, 16);
		// image.setPosRect(m_background.posRect().x + 10.5 * 3 + (i - 1) * 27 * 2,
		//                  m_background.posRect().y + 86.5 * 3,
		//                  16 * 2, 16 * 2);
		// target.draw(image, states);
}

