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
#include <gk/gl/RenderStates.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Color.hpp"
#include "Font.hpp"
#include "Text.hpp"

Text::Text() : m_font(gk::ResourceHandler::getInstance().get<Font>("font-ascii")) {
	m_vbo.layout().setupDefaultLayout();

	m_background.setFillColor(gk::Color::Transparent);
}

void Text::setString(const std::string &string) {
	if (m_string != string) {
		m_string = string;
		m_isUpdateNeeded = true;
	}
}

void Text::setColor(const gk::Color &color) {
	if (m_color != color) {
		m_color = color;
		m_isUpdateNeeded = true;
	}
}

void Text::setPadding(int x, int y) {
	if (m_padding.x != x || m_padding.y != y) {
		m_padding.x = x;
		m_padding.y = y;
		m_isUpdateNeeded = true;
	}
}

void Text::setMaxLineLength(unsigned int maxLineLength) {
	if (m_maxLineLength != maxLineLength) {
		m_maxLineLength = maxLineLength;
		m_isUpdateNeeded = true;
	}
}

void Text::draw(gk::RenderTarget &target, RenderStates states) const {
	if (m_isUpdateNeeded) {
		updateVertexBuffer();
		m_isUpdateNeeded = false;
	}

	states.transform *= getTransform();

	target.draw(m_background, states);

	if (m_verticesCount == 0) return;

	states.transform.translate((float)m_padding.x, (float)m_padding.y);
	states.texture = &m_font.texture();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	target.draw(m_vbo, GL_TRIANGLES, 0, m_verticesCount, states);
}

void Text::updateVertexBuffer() const {
	if (!m_isUpdateNeeded) return;

	std::vector<gk::Vertex> vertices;

	u32 x = 0;
	u32 y = 0;
	u32 maxX = 0;

	if (m_isShadowEnabled) {
		gk::Color color = m_shadowColor;
		for(char c : m_string) {
			if (c == '\n' || (m_maxLineLength && x + m_font.getCharWidth(c) >= m_maxLineLength)) {
				y += m_font.getTileSize().y + 1;
				x = 0;
				continue;
			}

			addCharacter(x + 1, y + 1, color, c, vertices);

			x += m_font.getCharWidth(c);
		}
	}

	x = 0;
	y = 0;
	gk::Color color = m_color;
	for(char c : m_string) {
		if (c == '\n' || (m_maxLineLength && x + m_font.getCharWidth(c) >= m_maxLineLength)) {
			maxX = std::max(x, maxX);
			y += m_font.getTileSize().y + 1;
			x = 0;
			continue;
		}

		if (c == '[')
			color = Color::Blue;

		addCharacter(x, y, color, c, vertices);

		if (c == ']')
			color = m_color;

		x += m_font.getCharWidth(c);
	}

	m_verticesCount = (u32)vertices.size();

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(gk::Vertex) * m_verticesCount, vertices.data(), GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);

	m_size.x = std::max(x, maxX);
	m_size.y = y + m_font.getTileSize().y + 1;

	float backgroundX = std::max(m_background.getSize().x, float(m_size.x + m_padding.x));
	float backgroundY = std::max(m_background.getSize().y, float(m_size.y + m_padding.y));

	m_background.setSize(backgroundX, backgroundY);
}

void Text::addCharacter(u32 x, u32 y, const gk::Color &color, u8 c, std::vector<gk::Vertex> &vertices) const {
	static const u8 coords[6][2] = {
		{1, 0},
		{0, 0},
		{1, 1},

		{1, 1},
		{0, 0},
		{0, 1},
	};

	for (int i = 0 ; i < 6 ; ++i) {
		vertices.emplace_back();
		gk::Vertex &vertex = vertices.back();

		vertex.coord3d[0] = float(x + coords[i][0] * m_font.getTileSize().x);
		vertex.coord3d[1] = float(y + coords[i][1] * m_font.getTileSize().y);
		vertex.coord3d[2] = 0;
		vertex.coord3d[3] = -1;

		vertex.color[0] = color.r;
		vertex.color[1] = color.g;
		vertex.color[2] = color.b;
		vertex.color[3] = color.a;

		gk::Vector2f texCoords = m_font.getTexCoords(c, coords[i][0], coords[i][1]);
		vertex.texCoord[0] = texCoords.x;
		vertex.texCoord[1] = texCoords.y;
	}
}

