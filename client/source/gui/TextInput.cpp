/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/resource/ResourceHandler.hpp>

#include "TextInput.hpp"

TextInput::TextInput() {
	m_background.setFillColor(gk::Color::Transparent);

	m_text.setText(std::string{m_cursor});
}

void TextInput::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && !m_content.empty()) {
		m_content.erase(m_content.begin() + m_content.length() - 1);

		m_text.setText(m_content + m_cursor);
	}

  	if (event.type == SDL_TEXTINPUT) {
		std::string text = event.text.text;
		for (char c : text) {
			if (isprint(c) && (!m_characterLimit || m_content.size() < m_characterLimit)) {
				m_content += c;
			}

			m_text.setText(m_content + m_cursor);
		}
	}
}

void TextInput::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);
	target.draw(m_text, states);
}

