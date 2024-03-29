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
#include "common/resource/ResourceHandler.hpp"

#include "client/graphics/Camera.hpp"
#include "client/graphics/PlayerBox.hpp"
#include "client/graphics/Texture.hpp"
#include "client/graphics/Vertex.hpp"

constexpr int NUM_QUADS = 34;
constexpr int NUM_INDICES_PER_QUAD = 6;
constexpr int NUM_VERTICES_PER_QUAD = 4;
constexpr int NUM_VERTEX_ELEMENTS = 5;

static constexpr float modelCoords[NUM_QUADS * NUM_VERTICES_PER_QUAD][NUM_VERTEX_ELEMENTS] = {
	// RIGHT LEG
	// West
	{-0.15f, -0.15f - 0.18f, 0.f,     16.f/64.f,  0.f/32.f},
	{-0.15f, -0.15f - 0.18f, 0.575f,  16.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f - 0.18f, 0.575f,  12.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f - 0.18f, 0.f,     12.f/64.f,  0.f/32.f},

	// East
	{ 0.15f, -0.15f - 0.18f, 0.575f,   4.f/64.f, 12.f/32.f},
	{ 0.15f, -0.15f - 0.18f, 0.f,      4.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f - 0.18f, 0.f,      8.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f - 0.18f, 0.575f,   8.f/64.f, 12.f/32.f},

	// South
	{-0.15f, -0.15f - 0.18f, 0.f,      0.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f - 0.18f, 0.f,      4.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f - 0.18f, 0.575f,   4.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f - 0.18f, 0.575f,  0.f/64.f, 12.f/32.f},

	// North
	{-0.15f,  0.15f - 0.18f, 0.575f,  12.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f - 0.18f, 0.575f,   8.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f - 0.18f, 0.f,      8.f/64.f,  0.f/32.f},
	{-0.15f,  0.15f - 0.18f, 0.f,     12.f/64.f,  0.f/32.f},

	// Bottom
	{ 0.15f, -0.15f - 0.18f, 0.f,      8.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f - 0.18f, 0.f,      8.f/64.f, 16.f/32.f},
	{-0.15f,  0.15f - 0.18f, 0.f,     12.f/64.f, 16.f/32.f},
	{ 0.15f,  0.15f - 0.18f, 0.f,     12.f/64.f, 12.f/32.f},

	// LEFT LEG
	// West
	{-0.15f, -0.15f + 0.18f, 0.f,     12.f/64.f,  0.f/32.f},
	{-0.15f, -0.15f + 0.18f, 0.575f,  12.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f + 0.18f, 0.575f,  16.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f + 0.18f, 0.f,     16.f/64.f,  0.f/32.f},

	// East
	{ 0.15f, -0.15f + 0.18f, 0.575f,   8.f/64.f, 12.f/32.f},
	{ 0.15f, -0.15f + 0.18f, 0.f,      8.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f + 0.18f, 0.f,      4.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f + 0.18f, 0.575f,   4.f/64.f, 12.f/32.f},

	// South
	{-0.15f, -0.15f + 0.18f, 0.f,     12.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f + 0.18f, 0.f,      8.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f + 0.18f, 0.575f,   8.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f + 0.18f, 0.575f,  12.f/64.f, 12.f/32.f},

	// North
	{-0.15f,  0.15f + 0.18f, 0.575f,   0.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f + 0.18f, 0.575f,   4.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f + 0.18f, 0.f,      4.f/64.f,  0.f/32.f},
	{-0.15f,  0.15f + 0.18f, 0.f,      0.f/64.f,  0.f/32.f},

	// Bottom
	{ 0.15f, -0.15f + 0.18f, 0.f,     12.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f + 0.18f, 0.f,     12.f/64.f, 16.f/32.f},
	{-0.15f,  0.15f + 0.18f, 0.f,      8.f/64.f, 16.f/32.f},
	{ 0.15f,  0.15f + 0.18f, 0.f,      8.f/64.f, 12.f/32.f},

	// BODY
	// West
	{-0.15f, -0.33f, 0.575f,  40.f/64.f,  0.f/32.f},
	{-0.15f, -0.33f, 1.35f,   40.f/64.f, 12.f/32.f},
	{-0.15f,  0.33f, 1.35f,   32.f/64.f, 12.f/32.f},
	{-0.15f,  0.33f, 0.575f,  32.f/64.f,  0.f/32.f},

	// East
	{ 0.15f, -0.33f, 1.35f,   20.f/64.f, 12.f/32.f},
	{ 0.15f, -0.33f, 0.575f,  20.f/64.f,  0.f/32.f},
	{ 0.15f,  0.33f, 0.575f,  28.f/64.f,  0.f/32.f},
	{ 0.15f,  0.33f, 1.35f,   28.f/64.f, 12.f/32.f},

	// South
	{-0.15f, -0.33f, 0.575f,  16.f/64.f,  0.f/32.f},
	{ 0.15f, -0.33f, 0.575f,  20.f/64.f,  0.f/32.f},
	{ 0.15f, -0.33f, 1.35f,   20.f/64.f, 12.f/32.f},
	{-0.15f, -0.33f, 1.35f,   16.f/64.f, 12.f/32.f},

	// North
	{-0.15f,  0.33f, 1.35f,   32.f/64.f, 12.f/32.f},
	{ 0.15f,  0.33f, 1.35f,   28.f/64.f, 12.f/32.f},
	{ 0.15f,  0.33f, 0.575f,  28.f/64.f,  0.f/32.f},
	{-0.15f,  0.33f, 0.575f,  32.f/64.f,  0.f/32.f},

	// Bottom
	{ 0.15f, -0.33f, 0.575f,  36.f/64.f, 12.f/32.f},
	{-0.15f, -0.33f, 0.575f,  36.f/64.f, 16.f/32.f},
	{-0.15f,  0.33f, 0.575f,  28.f/64.f, 16.f/32.f},
	{ 0.15f,  0.33f, 0.575f,  28.f/64.f, 12.f/32.f},

	// Top
	{-0.15f, -0.33f, 1.35f,   20.f/64.f, 16.f/32.f},
	{ 0.15f, -0.33f, 1.35f,   20.f/64.f, 12.f/32.f},
	{ 0.15f,  0.33f, 1.35f,   28.f/64.f, 12.f/32.f},
	{-0.15f,  0.33f, 1.35f,   28.f/64.f, 16.f/32.f},

	// RIGHT ARM
	// West
	{-0.15f, -0.15f - 0.48f, 0.675f,  56.f/64.f,  0.f/32.f},
	{-0.15f, -0.15f - 0.48f, 1.35f,   56.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f - 0.48f, 1.35f,   52.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f - 0.48f, 0.675f,  52.f/64.f,  0.f/32.f},

	// East
	{ 0.15f, -0.15f - 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},
	{ 0.15f, -0.15f - 0.48f, 0.675f,  44.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 0.675f,  48.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},

	// South
	{-0.15f, -0.15f - 0.48f, 0.675f,  40.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f - 0.48f, 0.675f,  44.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f - 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f - 0.48f, 1.35f,   40.f/64.f, 12.f/32.f},

	// North
	{-0.15f,  0.15f - 0.48f, 1.35f,   52.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 0.675f,  48.f/64.f,  0.f/32.f},
	{-0.15f,  0.15f - 0.48f, 0.675f,  52.f/64.f,  0.f/32.f},

	// Bottom
	{ 0.15f, -0.15f - 0.48f, 0.675f,  52.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f - 0.48f, 0.675f,  52.f/64.f, 16.f/32.f},
	{-0.15f,  0.15f - 0.48f, 0.675f,  48.f/64.f, 16.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 0.675f,  48.f/64.f, 12.f/32.f},

	// Top
	{-0.15f, -0.15f - 0.48f, 1.35f,   44.f/64.f, 16.f/32.f},
	{ 0.15f, -0.15f - 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f - 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f - 0.48f, 1.35f,   48.f/64.f, 16.f/32.f},

	// LEFT ARM
	// West
	{-0.15f, -0.15f + 0.48f, 0.675f,  52.f/64.f,  0.f/32.f},
	{-0.15f, -0.15f + 0.48f, 1.35f,   52.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f + 0.48f, 1.35f,   56.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f + 0.48f, 0.675f,  56.f/64.f,  0.f/32.f},

	// East
	{ 0.15f, -0.15f + 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},
	{ 0.15f, -0.15f + 0.48f, 0.675f,  48.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 0.675f,  44.f/64.f,  0.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},

	// South
	{-0.15f, -0.15f + 0.48f, 0.675f,  52.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f + 0.48f, 0.675f,  48.f/64.f,  0.f/32.f},
	{ 0.15f, -0.15f + 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f + 0.48f, 1.35f,   52.f/64.f, 12.f/32.f},

	// North
	{-0.15f,  0.15f + 0.48f, 1.35f,   40.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 0.675f,  44.f/64.f,  0.f/32.f},
	{-0.15f,  0.15f + 0.48f, 0.675f,  40.f/64.f,  0.f/32.f},

	// Bottom
	{ 0.15f, -0.15f + 0.48f, 0.675f,  48.f/64.f, 12.f/32.f},
	{-0.15f, -0.15f + 0.48f, 0.675f,  48.f/64.f, 16.f/32.f},
	{-0.15f,  0.15f + 0.48f, 0.675f,  52.f/64.f, 16.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 0.675f,  52.f/64.f, 12.f/32.f},

	// Top
	{-0.15f, -0.15f + 0.48f, 1.35f,   48.f/64.f, 16.f/32.f},
	{ 0.15f, -0.15f + 0.48f, 1.35f,   48.f/64.f, 12.f/32.f},
	{ 0.15f,  0.15f + 0.48f, 1.35f,   44.f/64.f, 12.f/32.f},
	{-0.15f,  0.15f + 0.48f, 1.35f,   44.f/64.f, 16.f/32.f},

	// HEAD
	// West
	{-0.2f, -0.2f, 1.35f,   32.f/64.f, 16.f/32.f},
	{-0.2f, -0.2f, 1.75f,   32.f/64.f, 24.f/32.f},
	{-0.2f,  0.2f, 1.75f,   24.f/64.f, 24.f/32.f},
	{-0.2f,  0.2f, 1.35f,   24.f/64.f, 16.f/32.f},

	// East
	{ 0.2f, -0.2f, 1.75f,    8.f/64.f, 24.f/32.f},
	{ 0.2f, -0.2f, 1.35f,    8.f/64.f, 16.f/32.f},
	{ 0.2f,  0.2f, 1.35f,   16.f/64.f, 16.f/32.f},
	{ 0.2f,  0.2f, 1.75f,   16.f/64.f, 24.f/32.f},

	// South
	{-0.2f, -0.2f, 1.35f,    0.f/64.f, 16.f/32.f},
	{ 0.2f, -0.2f, 1.35f,    8.f/64.f, 16.f/32.f},
	{ 0.2f, -0.2f, 1.75f,    8.f/64.f, 24.f/32.f},
	{-0.2f, -0.2f, 1.75f,    0.f/64.f, 24.f/32.f},

	// North
	{-0.2f,  0.2f, 1.75f,   24.f/64.f, 24.f/32.f},
	{ 0.2f,  0.2f, 1.75f,   16.f/64.f, 24.f/32.f},
	{ 0.2f,  0.2f, 1.35f,   16.f/64.f, 16.f/32.f},
	{-0.2f,  0.2f, 1.35f,   24.f/64.f, 16.f/32.f},

	// Bottom
	{ 0.2f, -0.2f, 1.35f,   24.f/64.f, 24.f/32.f},
	{-0.2f, -0.2f, 1.35f,   24.f/64.f, 32.f/32.f},
	{-0.2f,  0.2f, 1.35f,   16.f/64.f, 32.f/32.f},
	{ 0.2f,  0.2f, 1.35f,   16.f/64.f, 24.f/32.f},

	// Top
	{-0.2f, -0.2f, 1.75f,    8.f/64.f, 32.f/32.f},
	{ 0.2f, -0.2f, 1.75f,    8.f/64.f, 24.f/32.f},
	{ 0.2f,  0.2f, 1.75f,   16.f/64.f, 24.f/32.f},
	{-0.2f,  0.2f, 1.75f,   16.f/64.f, 32.f/32.f},
};

PlayerBox::PlayerBox(const Camera &camera)
	: m_camera(camera),
	  m_texture(ResourceHandler::getInstance().get<Texture>("texture-player"))
{
	m_vbo.setupDefaultLayout();

	initVertexBuffer();
	initIndexBuffer();
}

void PlayerBox::initVertexBuffer() {
	Vertex vertices[NUM_QUADS * NUM_VERTICES_PER_QUAD];
	for (u8 i = 0 ; i < NUM_QUADS * NUM_VERTICES_PER_QUAD ; ++i) {
		vertices[i].coord3d[0] = modelCoords[i][0];
		vertices[i].coord3d[1] = modelCoords[i][1];
		vertices[i].coord3d[2] = modelCoords[i][2];
		vertices[i].coord3d[3] = -1;

		vertices[i].texCoord[0] = modelCoords[i][3];
		vertices[i].texCoord[1] = 1.f - modelCoords[i][4];
	}

	m_vbo.init(vertices, sizeof(vertices));
}

void PlayerBox::initIndexBuffer() {
	u16 indices[NUM_QUADS * NUM_INDICES_PER_QUAD] = {
		0, 1, 2,
		2, 3, 0,
	};

	for (u32 i = 1; i < NUM_QUADS; ++i) {
		indices[i * NUM_INDICES_PER_QUAD + 0] = u16(indices[0] + i * NUM_VERTICES_PER_QUAD);
		indices[i * NUM_INDICES_PER_QUAD + 1] = u16(indices[1] + i * NUM_VERTICES_PER_QUAD);
		indices[i * NUM_INDICES_PER_QUAD + 2] = u16(indices[2] + i * NUM_VERTICES_PER_QUAD);
		indices[i * NUM_INDICES_PER_QUAD + 3] = u16(indices[3] + i * NUM_VERTICES_PER_QUAD);
		indices[i * NUM_INDICES_PER_QUAD + 4] = u16(indices[4] + i * NUM_VERTICES_PER_QUAD);
		indices[i * NUM_INDICES_PER_QUAD + 5] = u16(indices[5] + i * NUM_VERTICES_PER_QUAD);
	}

	m_ibo.init(indices, sizeof(indices));
}

void PlayerBox::draw(RenderTarget &target, RenderStates states) const {
	// Subtract the camera position - see comment in ClientWorld::draw()
	const Vector3d &cameraPosition = m_camera.getDPosition();
	states.transform.translate(
		static_cast<float>(m_x - cameraPosition.x),
		static_cast<float>(m_y - cameraPosition.y),
		static_cast<float>(m_z - cameraPosition.z)
	);
	states.transform.rotateZ(m_viewAngleH);

	states.transform *= getTransform();
	states.texture = &m_texture;

	target.drawElements(m_vbo, m_ibo, 0, 0, states);
}
