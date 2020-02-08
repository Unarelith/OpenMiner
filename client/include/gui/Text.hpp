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
#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>

#include <gk/graphics/Sprite.hpp>

class Text : public gk::Drawable, public gk::Transformable {
	public:
		Text();

		const std::string &text() const { return m_text; }
		void setText(const std::string &text);

		const gk::Color &color() const { return m_color; }
		void setColor(const gk::Color &color);

		const gk::Vector2i &getSize() const { return m_size; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void updateTextSprites();
		void updateCharWidth();

		std::string m_text;
		std::vector<gk::Sprite> m_textSprites;

		int m_charWidth[256];

		gk::Texture &m_texture;
		gk::VertexBuffer m_vbo;

		gk::Vector2i m_size;

		gk::Color m_color = gk::Color::White;
};

#endif // TEXT_HPP_
