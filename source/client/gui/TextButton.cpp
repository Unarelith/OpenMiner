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
#include "TextButton.hpp"

TextButton::TextButton(Widget *parent) : TextButton(200, parent) {
}

TextButton::TextButton(u16 width, Widget *parent) : Widget(width, 20, parent) {
	m_text.setColor(m_defaultColor);
	m_text.setShadowColor(gk::Color::fromRGBA32(56, 56, 56));

	m_background.setClipRect(0, 66, width, 20);
	m_hoverBackground.setClipRect(0, 86, width, 20);
	m_disabledBackground.setClipRect(0, 46, width, 20);

	m_backgroundBorder.setClipRect(200 - 2, 66, 2, 20);
	m_hoverBackgroundBorder.setClipRect(200 - 2, 86, 2, 20);
	m_disabledBackgroundBorder.setClipRect(200 - 2, 46, 2, 20);

	m_backgroundBorder.setPosition(width - 2.f, 0);
	m_hoverBackgroundBorder.setPosition(width - 2.f, 0);
	m_disabledBackgroundBorder.setPosition(width - 2.f, 0);
}

TextButton::TextButton(const CppCallback &callback, Widget *parent) : TextButton(parent) {
	m_cppCallback = callback;
}

void TextButton::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		m_isHovered = isPointInWidget(event.motion.x, event.motion.y);

		if (m_isEnabled && m_isHovered)
			m_text.setColor(m_hoverColor);
		else if (m_isEnabled && !m_isHovered)
			m_text.setColor(m_defaultColor);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && m_isHovered && m_isEnabled) {
		m_isHovered = isPointInWidget(event.button.x, event.button.y);

		if (m_isEnabled && m_isHovered)
			m_text.setColor(m_hoverColor);
		else if (m_isEnabled && !m_isHovered)
			m_text.setColor(m_defaultColor);

		if (m_isHovered) {
			if (m_cppCallback)
				m_cppCallback(*this);
			else if (m_luaCallback)
				m_luaCallback(*this);
		}
	}
}

void TextButton::setText(const std::string &text) {
	m_text.setString(text);
	m_text.updateVertexBuffer();
	m_text.setPosition(
		std::roundf((float)m_width  / 2.f - (float)m_text.getSize().x / 2.f),
		std::roundf((float)m_height / 2.f - (float)m_text.getSize().y / 2.f)
	);
}

void TextButton::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	if (!m_isEnabled) {
		target.draw(m_disabledBackground, states);
		target.draw(m_disabledBackgroundBorder, states);
	}
	else if (m_isHovered) {
		target.draw(m_hoverBackground, states);
		target.draw(m_hoverBackgroundBorder, states);
	}
	else {
		target.draw(m_background, states);
		target.draw(m_backgroundBorder, states);
	}

	target.draw(m_text, states);
}

