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
	reset(width, height);
}

void MenuWidget::reset(u16 width, u16 height) {
	m_width = width;
	m_height = height;

	m_buttons.clear();
	m_buttons.reserve(m_width * m_height);
}

void MenuWidget::onEvent(const SDL_Event &event) {
	for (std::size_t i = 0 ; i < m_buttons.size() ; ++i) {
		m_buttons.at(i).onEvent(event);

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			int x = i % m_width;
			int y = i / m_width;

			m_buttons.at(i).setPosition(SCREEN_WIDTH  / getScale().x / 2 - (m_width  * (m_buttons.at(i).width()  + s_horizontalSpacing) - s_horizontalSpacing) / 2 + x * (m_buttons.at(i).width() + s_horizontalSpacing),
			                            SCREEN_HEIGHT / getScale().y / 2 - (m_height * (m_buttons.at(i).height() + s_verticalSpacing)   - s_verticalSpacing)   / 2 + y * (m_buttons.at(i).height() + s_verticalSpacing), 0);
		}
	}
}

TextButton &MenuWidget::addButton(const std::string &text, const TextButton::CppCallback &callback) {
	int x = m_buttons.size() % m_width;
	int y = m_buttons.size() / m_width;

	m_buttons.emplace_back(this);

	TextButton &button = m_buttons.back();
	button.setText(text);
	button.setCallback(callback);
	button.setPosition(SCREEN_WIDTH  / getScale().x / 2 - (m_width  * (button.width()  + s_horizontalSpacing) - s_horizontalSpacing) / 2 + x * (button.width() + s_horizontalSpacing),
	                   SCREEN_HEIGHT / getScale().y / 2 - (m_height * (button.height() + s_verticalSpacing)   - s_verticalSpacing)   / 2 + y * (button.height() + s_verticalSpacing), 0);
	return button;
}

void MenuWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	for (const TextButton &button : m_buttons) {
		if (!button.text().empty())
			target.draw(button, states);
	}
}

