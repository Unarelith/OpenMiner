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

// FIXME
static const auto backgroundX = SCREEN_WIDTH / 2 - 182 * 3 / 2;
static const auto backgroundY = SCREEN_HEIGHT - 22 * 3;

Hotbar::Hotbar(Inventory &inventory) : m_inventory(inventory) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_background.load("texture-widgets");
	m_background.setClipRect(0, 0, 182, 22);
	m_background.setPosition(backgroundX, backgroundY, 0);
	m_background.setScale(3, 3, 1);

	m_cursor.load("texture-widgets");
	m_cursor.setClipRect(0, 22, 24, 24);
	m_cursor.setPosition(backgroundX - 3, backgroundY - 3, 0);
	m_cursor.setScale(3, 3, 1);

	for (u16 i = 0 ; i < 9 ; ++i) {
		ItemWidget &widget = m_items.emplace_back(m_inventory, i, 0);
		widget.setPosition(backgroundX + 16 + 180 / 3.0 * i - 9, backgroundY + 7, 0);
		widget.setScale(3, 3, 1);
	}
}

void Hotbar::update() {
	for (u16 i = 0 ; i < 9 ; ++i) {
		m_items[i].setStack(m_inventory.getStack(i, 0).item().id(), m_inventory.getStack(i, 0).amount());
	}
}

void Hotbar::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEWHEEL) {
		if (event.wheel.y == -1)
			m_cursorPos = (m_cursorPos + 1) % 9;
		else if (event.wheel.y == 1)
			m_cursorPos = (m_cursorPos == 0) ? 8 : m_cursorPos - 1;

		m_cursor.setPosition(backgroundX - 3 + 20 * 3 * m_cursorPos, backgroundY - 3, 0);
	}
}

void Hotbar::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;

	target.draw(m_background, states);

	for (auto &it : m_items)
		target.draw(it, states);

	target.draw(m_cursor, states);
}

