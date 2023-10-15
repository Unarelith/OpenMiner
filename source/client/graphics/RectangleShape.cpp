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
#include "RectangleShape.hpp"
#include "Vertex.hpp"

RectangleShape::RectangleShape() {
	m_vbo.setupDefaultLayout();

	std::array<u16, 6 * 5> m_indices{{
		0, 1, 3,
		3, 1, 2
	}};

	for (u8 i = 1 ; i < 5 ; ++i) {
		for (u8 j = 0 ; j < 6 ; ++j) {
			m_indices[i * 6 + j] = u16(m_indices[j] + 4 * i);
		}
	}

	m_ibo.init(m_indices.data(), 6 * 5 * sizeof(u16));
}

RectangleShape::RectangleShape(float width, float height, const gk::Color &color) : RectangleShape() {
	m_color = color;

	setSize(width, height);
}

void RectangleShape::updateVertexBuffer() {
	float o = (float)m_outlineThickness;
	Vertex vertices[4 + 4 * 4] = {
		// Rectangle vertices
		{{m_width, 0,        0, -1}},
		{{0,       0,        0, -1}},
		{{0,       m_height, 0, -1}},
		{{m_width, m_height, 0, -1}},

		// Top outline
		{{m_width + o, -o,   0, -1}},
		{{-o,          -o,   0, -1}},
		{{-o,          0,    0, -1}},
		{{m_width + o, 0,    0, -1}},

		// Bottom outline
		{{m_width + o, m_height,     0, -1}},
		{{-o,          m_height,     0, -1}},
		{{-o,          m_height + o, 0, -1}},
		{{m_width + o, m_height + o, 0, -1}},

		// Left outline
		{{0,      0,        0, -1}},
		{{-o,     0,        0, -1}},
		{{-o,     m_height, 0, -1}},
		{{0,      m_height, 0, -1}},

		// Right outline
		{{m_width,      0,        0, -1}},
		{{m_width + o,  0,        0, -1}},
		{{m_width + o,  m_height, 0, -1}},
		{{m_width,      m_height, 0, -1}},
	};

	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = m_color.r;
		vertices[i].color[1] = m_color.g;
		vertices[i].color[2] = m_color.b;
		vertices[i].color[3] = m_color.a;
	}

	for (u8 i = 4 ; i < 4 + 4 * 4 ; ++i) {
		vertices[i].color[0] = m_outlineColor.r;
		vertices[i].color[1] = m_outlineColor.g;
		vertices[i].color[2] = m_outlineColor.b;
		vertices[i].color[3] = m_outlineColor.a;
	}

	m_vbo.init(vertices, sizeof(vertices), true);
}

void RectangleShape::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

#ifdef OM_NOT_IMPLEMENTED
	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDisable(GL_DEPTH_TEST));
#endif // OM_NOT_IMPLEMENTED

#ifdef OM_NOT_IMPLEMENTED
	if(m_wireframeMode) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
#endif // OM_NOT_IMPLEMENTED
	target.drawElements(m_vbo, m_ibo, 0, 0, states);
#ifdef OM_NOT_IMPLEMENTED
	if(m_wireframeMode) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
#endif // OM_NOT_IMPLEMENTED
}
