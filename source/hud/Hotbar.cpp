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
}

void Hotbar::draw(RenderTarget &target, RenderStates states) const {
	target.draw(m_background, states);
}

