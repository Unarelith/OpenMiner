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
#include <gk/gl/Vertex.hpp>

#include "PlayerBox.hpp"

static const float cubeCoords[6 * 4 * 3] = {
	// West
	0, 0, 0,
	0, 0, 1,
	0, 1, 1,
	0, 1, 0,

	// East
	1, 0, 1,
	1, 0, 0,
	1, 1, 0,
	1, 1, 1,

	// South
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1,

	// North
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	0, 1, 0,

	// Bottom
	1, 0, 0,
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,

	// Top
	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1,
};

PlayerBox::PlayerBox() {
	setScale(1, 1, 2);

	updateVertexBuffer();
}

void PlayerBox::updateVertexBuffer() {
	gk::Vertex vertices[24];
	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = cubeCoords[i * 3];
		vertices[i].coord3d[1] = cubeCoords[i * 3 + 1];
		vertices[i].coord3d[2] = cubeCoords[i * 3 + 2] - 0.5;
		vertices[i].coord3d[3] = -1;

		vertices[i].color[0] = 0.3f;
		vertices[i].color[1] = 0.8f;
		vertices[i].color[2] = 0.7f;
		vertices[i].color[3] = 1.0f;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void PlayerBox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_vbo, GL_QUADS, 0, 24, states);
}

