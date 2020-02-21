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
#ifndef TEXTINPUT_HPP_
#define TEXTINPUT_HPP_

#include <gk/core/SDLHeaders.hpp>

#include "Text.hpp"

class TextInput : public gk::Drawable, public gk::Transformable {
	public:
		TextInput();

		void onEvent(const SDL_Event &event);

		const std::string &text() const { return m_content; }
		void setText(const std::string &text) { m_content = text; m_text.setText(m_content + m_cursor); }

		void setBackgroundColor(const gk::Color &color) { m_text.setBackgroundColor(color); }
		void setBackgroundSize(unsigned int width, unsigned int height) { m_text.setBackgroundSize(width, height); }

		void setPadding(int x, int y) { m_text.setPadding(x, y); }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Text m_text;
		std::string m_content;
		char m_cursor = '_';

		u16 m_characterLimit = 0;
};

#endif // TEXTINPUT_HPP_
