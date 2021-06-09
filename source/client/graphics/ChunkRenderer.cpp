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
#include <gk/gl/Shader.hpp>
#include <gk/gl/Texture.hpp>

#include "ChunkRenderer.hpp"
#include "ClientChunk.hpp"

void ChunkRenderer::draw(gk::RenderTarget &target, gk::RenderStates states, const std::vector<std::pair<ClientChunk*, gk::Transform>> &chunks) {
	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	glCheck(glEnable(GL_DEPTH_TEST));

	states.texture = &m_textureAtlas.texture();
	target.beginSceneDraw(states);

	for (u8 layer = 0 ; layer < ChunkMeshLayer::Count ; ++layer) {
		// Disable mipmaps for specific layers
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,
			(layer == ChunkMeshLayer::NoMipMap || layer == ChunkMeshLayer::Flora) ? 0 : Config::mipmapLevels));

		if (layer == ChunkMeshLayer::Flora)
			glCheck(glDisable(GL_CULL_FACE));
		else
			glCheck(glEnable(GL_CULL_FACE));

		for (auto &it : chunks) {
			states.shader->setUniform("u_modelMatrix", it.second);

			std::size_t verticesCount = it.first->getVerticesCount(layer);
			if (verticesCount == 0) continue;

			target.drawVertexBuffer(it.first->getVBO(layer), GL_TRIANGLES, 0, (GLsizei)verticesCount, states);

			it.first->setHasBeenDrawn(true);
		}
	}

	target.endSceneDraw(states);

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

