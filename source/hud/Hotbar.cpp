/*
 * =====================================================================================
 *
 *       Filename:  Hotbar.cpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 05:40:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Hotbar.hpp"

Hotbar::Hotbar() {
	m_texture.load("textures/widgets.png");

	m_background.load(m_texture);
	m_background.setClipRect(0, 0, 182, 22);
	m_background.setPosRect(SCREEN_WIDTH / 2 - 182 * 3 / 2, SCREEN_HEIGHT - 22 * 3, 182 * 3, 22 * 3);

	m_cursor.load(m_texture);
	m_cursor.setClipRect(0, 22, 24, 24);
	m_cursor.setPosRect(m_background.posRect().x - 3, m_background.posRect().y - 3, 24 * 3, 24 * 3);

	m_blocksTexture.load("textures/blocks.png");

	// for (u16 i = 1 ; i < 9 ; ++i)
	// 	addItem(i);
}

void Hotbar::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEWHEEL) {
		if (event.wheel.y == -1)
			m_cursorPos = (m_cursorPos + 1) % 9;
		else if (event.wheel.y == 1)
			m_cursorPos = (m_cursorPos == 0) ? 8 : m_cursorPos - 1;

		m_cursor.setPosRect(m_background.posRect().x - 3 + 20 * 3 * m_cursorPos, m_background.posRect().y - 3, 24 * 3, 24 * 3);
	}
}

// void Hotbar::addItem(u16 id) {
// 	Image &image = m_items.emplace_back();
// 	image.load(m_blocksTexture);
// 	image.setClipRect(id * 16, 0, 16, 16);
// 	image.setPosRect(m_background.posRect().x + 16 + 182 * 3 / 9 * id, m_background.posRect().y + 16, 16 * 2, 16 * 2);
// }

void Hotbar::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_background, states);

	// for (auto &it : m_items)
	// 	target.draw(it, states);

	for (u16 i = 1 ; i < 10 ; ++i) {
		Image image;
		image.load(m_blocksTexture);
		image.setClipRect(i * 16, 0, 16, 16);
		image.setPosRect(m_background.posRect().x + 16 + 182 * 3 / 9 * (i - 1), m_background.posRect().y + 16, 16 * 2, 16 * 2);
		target.draw(image, states);
	}

	target.draw(m_cursor, states);
}

