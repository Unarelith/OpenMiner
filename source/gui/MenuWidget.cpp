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

MenuWidget::MenuWidget(u16 width, u16 height, Widget *parent) : Widget(parent) {
	m_width = width;
	m_height = height;

	m_buttons.resize(m_width * m_height);
}

void MenuWidget::onEvent(const SDL_Event &event) {
	for (TextButton &button : m_buttons) {
		button.onEvent(event);
	}
}

TextButton &MenuWidget::addButton(u16 x, u16 y, const std::string &text, const TextButton::Callback &callback) {
	TextButton &button = m_buttons.at(x + y);
	button.setParent(this);
	button.setText(text);
	button.setCallback(callback);
	button.setPosition(SCREEN_WIDTH  / getScale().x / 2 - (m_width  * (button.width()  + s_horizontalSpacing) - s_horizontalSpacing) / 2 + x * (button.width() + s_horizontalSpacing),
	                   SCREEN_HEIGHT / getScale().y / 2 - (m_height * (button.height() + s_verticalSpacing)   - s_verticalSpacing)   / 2 + y * (button.height() + s_verticalSpacing), 0);
	return button;
}

void MenuWidget::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	for (const TextButton &button : m_buttons) {
		if (!button.text().empty())
			target.draw(button, states);
	}
}

