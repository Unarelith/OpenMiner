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
#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>
#include <vector>

#include "RectangleShape.hpp"

class Font;
class Vertex;

class Text : public Drawable, public Transformable {
	public:
		Text();

		const std::string &string() const { return m_string; }
		void setString(const std::string &string);

		const Color &color() const { return m_color; }
		void setColor(const Color &color);
		void setShadowColor(const Color &color) { m_shadowColor = color; }

		const gk::Vector2i &getSize() const { return m_size; }
		gk::Vector2f getBackgroundSize() const { return m_background.getSize(); }

		void setBackgroundColor(const Color &color) { m_background.setFillColor(color); }
		void setBackgroundSize(unsigned int width, unsigned int height) { m_background.setSize((float)width, (float)height); }
		void setBackgroundOutline(int thickness, const Color &color) { m_background.setOutlineThickness(thickness); m_background.setOutlineColor(color); }
		void setShadowEnabled(bool isShadowEnabled) { m_isShadowEnabled = isShadowEnabled; m_isUpdateNeeded = true; }

		void setPadding(int x, int y);

		void setMaxLineLength(u32 maxLineLength);

		void updateVertexBuffer() const;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		void addCharacter(u32 x, u32 y, const Color &color, u8 c, std::vector<Vertex> &vertices) const;

		std::string m_string;

		bool m_isShadowEnabled = true;
		Color m_shadowColor = Color::fromRGBA32(70, 70, 70);

		Font &m_font;

		mutable VertexBuffer m_vbo;
		mutable u32 m_verticesCount = 0;
		mutable bool m_isUpdateNeeded = true;

		mutable gk::Vector2i m_size{0, 0};
		gk::Vector2i m_padding{0, 0};

		Color m_color = Color::White;

		mutable RectangleShape m_background;

		u32 m_maxLineLength = 0;
};

#endif // TEXT_HPP_
