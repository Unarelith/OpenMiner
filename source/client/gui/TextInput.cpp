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
#include <gk/resource/ResourceHandler.hpp>

#include "TextInput.hpp"

TextInput::TextInput(Widget *parent) : Widget(parent) {
	m_cursor.setString("_");

	m_placeholder.setColor(gk::Color::fromRGBA32(150, 150, 150));
}

void TextInput::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		m_hasFocus = isPointInWidget(event.button.x, event.button.y);
	}
	else if (m_hasFocus) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && !m_content.empty()) {
			m_content.erase(m_content.begin() + m_content.length() - 1);

			m_text.setString(m_content);
			m_text.updateVertexBuffer();
			m_cursor.setPosition((float)m_text.getSize().x, 0.f);
		}

		if (event.type == SDL_TEXTINPUT) {
			std::string text = event.text.text;
			for (char c : text) {
				if (isprint(c) && (!m_characterLimit || m_content.size() < m_characterLimit)) {
					m_content += c;
				}

				m_text.setString(m_content);
				m_text.updateVertexBuffer();
				m_cursor.setPosition((float)m_text.getSize().x, 0.f);
			}
		}
	}
}

void TextInput::setString(const std::string &string) {
	m_content = string;
	m_text.setString(m_content);
	m_text.updateVertexBuffer();
	m_cursor.setPosition((float)m_text.getSize().x, 0);
}

void TextInput::draw(gk::RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_text, states);

	if (m_hasFocus)
		target.draw(m_cursor, states);

	if (m_content.empty() && !m_hasFocus)
		target.draw(m_placeholder, states);
}

