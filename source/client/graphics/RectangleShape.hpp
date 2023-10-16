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
#ifndef RECTANGLESHAPE_HPP_
#define RECTANGLESHAPE_HPP_

#include <array>

#include <gk/graphics/Color.hpp>
#include <gk/gl/Transformable.hpp>

#include "Drawable.hpp"
#include "VertexBuffer.hpp"

class RectangleShape : public Drawable, public gk::Transformable {
	public:
		RectangleShape();
		RectangleShape(float width, float height, const gk::Color &color = gk::Color::White);

		const gk::Color &color() const { return m_color; }
		void setFillColor(const gk::Color &color) { m_color = color; updateVertexBuffer(); }

		float width() const { return m_width; }
		float height() const { return m_height; }

		gk::Vector2f getSize() const { return gk::Vector2f{m_width, m_height}; }

		void setSize(float width, float height) { m_width = width; m_height = height; updateVertexBuffer(); }
		void setSize(const gk::Vector2f &size) { m_width = size.x; m_height = size.y; updateVertexBuffer(); }

		int outlineThickness() const { return m_outlineThickness; }

		void setOutlineColor(const gk::Color &color) { m_outlineColor = color; updateVertexBuffer(); }
		void setOutlineThickness(int outlineThickness) { m_outlineThickness = outlineThickness; updateVertexBuffer(); }

	private:
		void updateVertexBuffer();

		void draw(RenderTarget &target, RenderStates states) const override;

		gk::Color m_color;

		float m_width = 0;
		float m_height = 0;

		VertexBuffer m_vbo;

		gk::Color m_outlineColor{gk::Color::White};
		int m_outlineThickness = 0;
};

#endif // RECTANGLESHAPE_HPP_
