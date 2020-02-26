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

constexpr int NUM_QUADS = 6;

static const float modelCoords[NUM_QUADS * 4 * 3] = {
	// West
	-0.3125f, -0.3125f, 0.f,
	-0.3125f, -0.3125f, 1.75f,
	-0.3125f,  0.3125f, 1.75f,
	-0.3125f,  0.3125f, 0.f,

	// East
	 0.3125f, -0.3125f, 1.75f,
	 0.3125f, -0.3125f, 0.f,
	 0.3125f,  0.3125f, 0.f,
	 0.3125f,  0.3125f, 1.75f,

	// South
	-0.3125f, -0.3125f, 0.f,
	 0.3125f, -0.3125f, 0.f,
	 0.3125f, -0.3125f, 1.75f,
	-0.3125f, -0.3125f, 1.75f,

	// North
	-0.3125f,  0.3125f, 1.75f,
	 0.3125f,  0.3125f, 1.75f,
	 0.3125f,  0.3125f, 0.f,
	-0.3125f,  0.3125f, 0.f,

	// Bottom
	 0.3125f, -0.3125f, 0.f,
	-0.3125f, -0.3125f, 0.f,
	-0.3125f,  0.3125f, 0.f,
	 0.3125f,  0.3125f, 0.f,

	// Top
	-0.3125f, -0.3125f, 1.75f,
	 0.3125f, -0.3125f, 1.75f,
	 0.3125f,  0.3125f, 1.75f,
	-0.3125f,  0.3125f, 1.75f,
};

PlayerBox::PlayerBox(const gk::Camera &camera) : m_camera(camera) {
	updateVertexBuffer();
}

void PlayerBox::updateVertexBuffer() {
	gk::Vertex vertices[NUM_QUADS * 4];
	for (u8 i = 0 ; i < NUM_QUADS * 4 ; ++i) {
		vertices[i].coord3d[0] = modelCoords[i * 3];
		vertices[i].coord3d[1] = modelCoords[i * 3 + 1];
		vertices[i].coord3d[2] = modelCoords[i * 3 + 2];
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
	// Subtract the camera position - see comment in ClientWorld::draw()
	gk::Vector3d cameraPosition = m_camera.getDPosition();
	states.transform.translate(m_x - cameraPosition.x, m_y - cameraPosition.y, m_z - cameraPosition.z);

	states.transform *= getTransform();

	target.draw(m_vbo, GL_QUADS, 0, NUM_QUADS * 4, states);
}

