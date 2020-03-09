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
#include <cmath>
#include <vector>

#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Texture.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "CloudRenderer.hpp"

// Same order as enum BlockFace in TilesDef.hpp
static float cubeCoords[6 * 4 * 3] = {
	// West
	0, 1, 0,
	0, 0, 0,
	0, 0, 1,
	0, 1, 1,

	// East
	1, 0, 0,
	1, 1, 0,
	1, 1, 1,
	1, 0, 1,

	// South
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1,

	// North
	1, 1, 0,
	0, 1, 0,
	0, 1, 1,
	1, 1, 1,

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

void CloudRenderer::initCloudMap() {
	gk::Texture &texture = gk::ResourceHandler::getInstance().get<gk::Texture>("texture-clouds");
	const int width = texture.getSize().x;
	const int height = texture.getSize().y;
	unsigned int *data = new unsigned int[width * height];

	gk::Texture::bind(&texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
	gk::Texture::bind(nullptr);

	std::memset(m_cloudMap, 0, CLOUD_MAP_SIZE * CLOUD_MAP_SIZE);

	for (u16 x = 0 ; x < CLOUD_MAP_SIZE ; ++x) {
		for (u16 z = 0 ; z < CLOUD_MAP_SIZE ; ++z) {
			u32 color = data[x + z * CLOUD_MAP_SIZE];
			if ((color & 0x000000ff) > 1)
				m_cloudMap[x][z] = 1;
		}
	}
}

void CloudRenderer::initVertexBuffer() {
	std::vector<gk::Vertex> vertices;

	for (u16 x = 0 ; x < CLOUD_MAP_SIZE ; ++x) {
		for (u16 y = 0 ; y < CLOUD_MAP_SIZE ; ++y) {
			if (m_cloudMap[x][y]) {
				const int surroundingBlocksPos[4][2] = {
					{x - 1, y},
					{x + 1, y},
					{x, y - 1},
					{x, y + 1},
				};

				for (u8 i = 0 ; i < 6 ; ++i) {
					if (i < 4) {
						s16 sx = surroundingBlocksPos[i][0];
						s16 sy = surroundingBlocksPos[i][1];
						if (sx >= 0 && sx < CLOUD_MAP_SIZE && sy >= 0 && sy < CLOUD_MAP_SIZE && m_cloudMap[sx][sy])
							continue;
					}

					gk::Vertex faceVertices[4];
					for(u8 j = 0 ; j < 4 ; ++j) {
						faceVertices[j].coord3d[0] = (x + cubeCoords[i * 12 + j * 3]) * 16;
						faceVertices[j].coord3d[1] = (y + cubeCoords[i * 12 + j * 3 + 1]) * 16;
						faceVertices[j].coord3d[2] = CLOUD_HEIGHT + 4 * cubeCoords[i * 12 + j * 3 + 2];
						faceVertices[j].coord3d[3] = -1;

						float color = 0.9;
						if (i == 0 || i == 1)
							color = 0.85;
						else if (i == 2 || i == 3)
							color = 0.8;

						faceVertices[j].color[0] = color;
						faceVertices[j].color[1] = color;
						faceVertices[j].color[2] = color;
						faceVertices[j].color[3] = 0.7;
					}

					vertices.emplace_back(faceVertices[0]);
					vertices.emplace_back(faceVertices[1]);
					vertices.emplace_back(faceVertices[2]);
					vertices.emplace_back(faceVertices[2]);
					vertices.emplace_back(faceVertices[3]);
					vertices.emplace_back(faceVertices[0]);
				}
			}
		}
	}

	m_verticesCount = vertices.size();

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(m_verticesCount * sizeof(gk::Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void CloudRenderer::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	gk::Vector3d cameraPosition = m_camera.getDPosition();
	states.transform.translate(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z);

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_DEPTH_TEST));
	// glCheck(glDisable(GL_POLYGON_OFFSET_FILL));
	// glCheck(glDisable(GL_CULL_FACE));

	// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	target.draw(m_vbo, GL_TRIANGLES, 0, m_verticesCount, states);
	// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

