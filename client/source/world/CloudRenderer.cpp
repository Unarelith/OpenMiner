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
#include <cmath>
#include <vector>

#include <gk/core/Debug.hpp> // FIXME

#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Vertex.hpp>

#include "CloudRenderer.hpp"

static const float cubeCoords[6 * 4 * 3] = {
	// Top
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	0, 1, 0,

	// Bottom
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1,

	// Left
	0, 0, 0,
	0, 0, 1,
	0, 1, 1,
	0, 1, 0,

	// Right
	1, 0, 1,
	1, 0, 0,
	1, 1, 0,
	1, 1, 1,

	// Front
	1, 0, 0,
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,

	// Back
	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1,
};

void CloudRenderer::initCloudMap() {
	for (u16 x = 0 ; x < CLOUD_MAP_SIZE ; ++x) {
		for (u16 z = 0 ; z < CLOUD_MAP_SIZE ; ++z) {
			m_cloudMap[x][z] = rand() % 2;
		}
	}
}

void CloudRenderer::initVertexBuffer() {
	std::vector<gk::Vertex> vertices;

	for (u16 x = 0 ; x < CLOUD_MAP_SIZE ; ++x) {
		for (u16 z = 0 ; z < CLOUD_MAP_SIZE ; ++z) {
			if (m_cloudMap[x][z]) {
				const int surroundingBlocksPos[4][2] = {
					{x - 1, z},
					{x + 1, z},
					{x, z - 1},
					{x, z + 1},
				};

				for (u8 i = 0 ; i < 6 ; ++i) {
					if (i > 1) {
						s16 sx = surroundingBlocksPos[i - 2][0];
						s16 sz = surroundingBlocksPos[i - 2][1];
						if (sx >= 0 && sx < CLOUD_MAP_SIZE && sz >= 0 && sz < CLOUD_MAP_SIZE && m_cloudMap[sx][sz])
							continue;
					}

					gk::Vertex faceVertices[4];
					for(u8 j = 0 ; j < 4 ; ++j) {
						faceVertices[j].coord3d[0] = (x + cubeCoords[i * 12 + j * 3]) * 4;
						faceVertices[j].coord3d[1] = CLOUD_HEIGHT + 4 * cubeCoords[i * 12 + j * 3 + 1];
						faceVertices[j].coord3d[2] = (z + cubeCoords[i * 12 + j * 3 + 2]) * 4;
						faceVertices[j].coord3d[3] = i;

						faceVertices[j].color[0] = 0.9;
						faceVertices[j].color[1] = 0.9;
						faceVertices[j].color[2] = 0.9;
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
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_DEPTH_TEST));
	// glCheck(glDisable(GL_POLYGON_OFFSET_FILL));
	// glCheck(glDisable(GL_CULL_FACE));

	// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	target.draw(m_vbo, GL_TRIANGLES, 0, m_verticesCount, states);
	// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

