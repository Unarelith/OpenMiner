/*
 * =====================================================================================
 *
 *       Filename:  TextButton.cpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:18:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "TextButton.hpp"

TextButton::TextButton(Widget *parent) : Widget(200, 20, parent) {
	// m_text.setFont("font-default");
	// m_text.setCharacterSize(8);
    //
	// m_textShadow.setFont("font-default");
	// m_textShadow.setCharacterSize(8);
	// m_textShadow.setColor(gk::Color{70, 70, 70, 255});

	m_background.setClipRect(0, 66, 200, 20);
	m_hoverBackground.setClipRect(0, 86, 200, 20);
	m_disabledBackground.setClipRect(0, 46, 200, 20);
}

TextButton::TextButton(const CppCallback &callback, Widget *parent) : TextButton(parent) {
	m_cppCallback = callback;
}

void TextButton::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		m_isHovered = isPointInWidget(event.motion.x, event.motion.y);

		if (m_isEnabled && m_isHovered && m_text.color() == gk::Color::White)
			m_text.setColor({255, 255, 160});
		else if (!m_isHovered && m_text.color() != gk::Color::White)
			m_text.setColor(gk::Color::White);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && m_isHovered && m_isEnabled) {
		if (m_cppCallback)
			m_cppCallback(*this);
		else if (m_luaCallback)
			m_luaCallback(*this);
	}
}

void TextButton::setText(const std::string &text) {
	m_text.setText(text);
	m_text.setPosition(m_width / 2 - m_text.getSize().x / 2,
			m_height / 2 - m_text.getSize().y / 2, 0);

	// m_text.setString(text);
	// m_text.setPosition(m_width / 2 - m_text.getLocalBounds().width / 2,
	// 		m_height / 2 - m_text.getLocalBounds().height / 2 - 1, 0);

	// m_textShadow.setString(text);
	// m_textShadow.setPosition(m_text.getPosition() + gk::Vector3f{1.f, 1.f, 0.f});
}

void TextButton::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	if (!m_isEnabled)
		target.draw(m_disabledBackground, states);
	else if (m_isHovered)
		target.draw(m_hoverBackground, states);
	else
		target.draw(m_background, states);

	// target.draw(m_textShadow, states);
	target.draw(m_text, states);
}

