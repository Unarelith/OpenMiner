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
#ifndef TEXTINPUT_HPP_
#define TEXTINPUT_HPP_

#include <SFML/Window/Event.hpp>

#include "Text.hpp"

class TextInput : public gk::Drawable, public gk::Transformable {
	public:
		TextInput();

		void onEvent(const sf::Event &event);

		const std::string &text() const { return m_content; }
		void setText(const std::string &text) { m_content = text; m_text.setText(m_content + m_cursor); }

		gk::Vector2f getBackgroundSize() const { return m_text.getBackgroundSize(); }

		void setBackgroundColor(const gk::Color &color) { m_text.setBackgroundColor(color); }
		void setBackgroundSize(unsigned int width, unsigned int height) { m_text.setBackgroundSize(width, height); }
		void setBackgroundOutline(int thickness, const gk::Color &color) { m_text.setBackgroundOutline(thickness, color); }

		void setPadding(int x, int y) { m_text.setPadding(x, y); }
		void setCharacterLimit(u16 characterLimit) { m_characterLimit = characterLimit; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Text m_text;
		std::string m_content;
		char m_cursor = '_';

		u16 m_characterLimit = 0;
};

#endif // TEXTINPUT_HPP_
