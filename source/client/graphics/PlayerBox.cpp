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
#include <gk/gl/GLCheck.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "PlayerBox.hpp"
#include "Vertex.hpp"

constexpr int NUM_QUADS = 34;
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

PlayerBox::PlayerBox(const gk::Camera &camera)
	: m_camera(camera),
	  m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-player"))
{
	m_vbo.layout().addAttribute(0, "coord3d", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	m_vbo.layout().addAttribute(1, "texCoord", 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	m_vbo.layout().addAttribute(2, "color", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
	m_vbo.layout().addAttribute(3, "normal", 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
	m_vbo.layout().addAttribute(4, "lightValue", 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, lightValue)));
	m_vbo.layout().addAttribute(5, "ambientOcclusion", 1, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, ambientOcclusion)));

	updateVertexBuffer();
}

void PlayerBox::updateVertexBuffer() {
	Vertex vertices[NUM_QUADS * NUM_VERTICES_PER_QUAD];
	for (u8 i = 0 ; i < NUM_QUADS * NUM_VERTICES_PER_QUAD ; ++i) {
		vertices[i].coord3d[0] = modelCoords[i][0];
		vertices[i].coord3d[1] = modelCoords[i][1];
		vertices[i].coord3d[2] = modelCoords[i][2];
		vertices[i].coord3d[3] = -1;

		vertices[i].texCoord[0] = modelCoords[i][3];
		vertices[i].texCoord[1] = 1.f - modelCoords[i][4];
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void PlayerBox::draw(RenderTarget &target, RenderStates states) const {
	// Subtract the camera position - see comment in ClientWorld::draw()
	const gk::Vector3d &cameraPosition = m_camera.getDPosition();
	states.transform.translate(
		static_cast<float>(m_x - cameraPosition.x),
		static_cast<float>(m_y - cameraPosition.y),
		static_cast<float>(m_z - cameraPosition.z)
	);
	states.transform.rotateZ(m_viewAngleH);

	states.transform *= getTransform();
	states.texture = &m_texture;

	glCheck(glEnable(GL_CULL_FACE));

	target.draw(m_vbo, GL_QUADS, 0, NUM_QUADS * NUM_VERTICES_PER_QUAD, states);
}

