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

TextInput::TextInput() {
	m_cursor.setString("_");

	m_placeholder.setColor(gk::Color(150, 150, 150));
}

void TextInput::onEvent(const sf::Event &event) {
	if (m_hasFocus) {
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace && !m_content.empty()) {
			m_content.erase(m_content.begin() + m_content.length() - 1);

			m_text.setString(m_content);
			m_text.updateVertexBuffer();
			m_cursor.setPosition(m_text.getSize().x, 0);
		}

		if (event.type == sf::Event::TextEntered) {
			u32 c = event.text.unicode;
			if (isprint(c) && (!m_characterLimit || m_content.size() < m_characterLimit)) {
				m_content += c;
			}

			m_text.setString(m_content);
			m_text.updateVertexBuffer();
			m_cursor.setPosition(m_text.getSize().x, 0);
		}
	}
}

void TextInput::setString(const std::string &string) {
	m_content = string;
	m_text.setString(m_content);
	m_text.updateVertexBuffer();
	m_cursor.setPosition(m_text.getSize().x, 0);
}

void TextInput::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_text, states);

	if (m_hasFocus)
		target.draw(m_cursor, states);

	if (m_content.empty() && !m_hasFocus)
		target.draw(m_placeholder, states);
}

