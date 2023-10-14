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

#include <gk/core/SDLHeaders.hpp>

#include "Text.hpp"
#include "Widget.hpp"

class TextInput : public Widget {
	public:
		TextInput(Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		const std::string &string() const { return m_content; }
		void setString(const std::string &string);

		gk::Vector2f getBackgroundSize() const { return m_text.getBackgroundSize(); }

		void setBackgroundColor(const gk::Color &color) { m_text.setBackgroundColor(color); }
		void setBackgroundOutline(int thickness, const gk::Color &color) { m_text.setBackgroundOutline(thickness, color); }
		void setBackgroundSize(unsigned int width, unsigned int height) {
			m_text.setBackgroundSize(width, height);
			m_width = width;
			m_height = height;
		}

		void setPadding(int x, int y) { m_text.setPadding(x, y); m_cursor.setPadding(x, y); m_placeholder.setPadding(x, y); }
		void setCharacterLimit(u16 characterLimit) { m_characterLimit = characterLimit; }

		void setPlaceholder(const std::string &placeholder) { m_placeholder.setString(placeholder); }
		void setPlaceholderColor(const gk::Color &color) { m_placeholder.setColor(color); }

		bool hasFocus() const { return m_hasFocus; }
		void setFocus(bool hasFocus) { m_hasFocus = hasFocus; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Text m_text;
		std::string m_content;

		Text m_cursor;

		u16 m_characterLimit = 0;

		Text m_placeholder;

		bool m_hasFocus = true;
};

#endif // TEXTINPUT_HPP_
