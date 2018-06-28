/*
 * =====================================================================================
 *
 *       Filename:  MenuWidget.cpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:33:22
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "MenuWidget.hpp"

void MenuWidget::onEvent(const SDL_Event &event) {
	for (TextButton &button : m_buttons) {
		button.onEvent(event);
	}
}

void MenuWidget::addButton(const std::string &text, const TextButton::Callback &callback) {
	TextButton &button = m_buttons.emplace_back(callback, this);
	button.setText(text);
	button.setPosition(SCREEN_WIDTH  / getScale().x / 2 - button.width() / 2,
	                   SCREEN_HEIGHT / getScale().y / 2 - button.height() / 2, 0);
}

void MenuWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	for (const TextButton &button : m_buttons) {
		target.draw(button, states);
	}
}

