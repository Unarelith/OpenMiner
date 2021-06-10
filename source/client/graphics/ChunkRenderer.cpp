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
#include <glm/gtx/norm.hpp>

#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/gl/Texture.hpp>

#include "ChunkRenderer.hpp"
#include "ClientChunk.hpp"

void ChunkRenderer::draw(gk::RenderTarget &target, gk::RenderStates states, const ChunkMap &chunks, gk::Camera &camera, const Sky *currentSky) const {
	// Changing the values sent to the GPU to double precision is suicidal,
	// performance wise, if possible at all. Therefore we want to keep the
	// GL rendering numbers in single precision format. But that introduces
	// an issue at larger coordinates, because the precision of floats
	// quickly degrades as the numbers grow, with a random wobbling being
	// very noticeable at e.g. coordinates >= 65536 or so, and the waving
	// leaves effect being very jerky in comparison with the effect near the
	// origin.
	//
	// To gain rendering precision, we subtract the camera position from the
	// coordinates of the models to be rendered, to make them all small in
	// relation to the camera, prior to converting them to floats. Then the
	// camera itself is moved to (0, 0, 0) for rendering purposes. Now the
	// vertex coordinates passed to the renderer are all small, and single
	// precision floats suffice for the drawing.

	gk::Vector3d cameraPos(camera.getDPosition());
	camera.setDPosition(0, 0, 0);  // Temporarily move the camera to the origin

	// Prepare a list of chunks to draw
	std::vector<std::pair<ClientChunk*, gk::Transform>> chunksToDraw;
	for(auto &it : chunks) {
		it.second->setHasBeenDrawn(false);

		gk::Transform tf = glm::translate(glm::mat4(1.0f),
		                                  glm::vec3(it.second->x() * CHUNK_WIDTH  - cameraPos.x,
		                                            it.second->y() * CHUNK_DEPTH  - cameraPos.y,
		                                            it.second->z() * CHUNK_HEIGHT - cameraPos.z));

		// Is the chunk close enough?
		glm::vec4 center = target.getView()->getViewTransform().getMatrix()
		                 * tf.getMatrix()
		                 * glm::vec4(CHUNK_WIDTH / 2, CHUNK_DEPTH / 2, CHUNK_HEIGHT / 2, 1);

		// Nope, too far, don't render it
		if(glm::length(center) > (Config::renderDistance + 1.f) * CHUNK_WIDTH) {
			if(floor(glm::length(center)) > (Config::renderDistance + 2.f) * CHUNK_WIDTH) {
				it.second->setTooFar(true);

				if (!it.second->isInitialized())
					m_onChunkDeletionRequested(gk::Vector3i{it.second->x(), it.second->y(), it.second->z()});
			}

			continue;
		}

		it.second->setTooFar(false);

		if (!it.second->isInitialized()) continue;

		// Is this chunk's centre on the screen?
		float d = glm::length2(center);
		center.x /= center.w;
		center.y /= center.w;

		// If it is behind the camera, don't bother drawing it.
		// Our screen coordinates are +X right, +Y up, and for a right-handed
		// coordinate system, depth must be negative Z, so anything with a
		// positive Z is behind the camera.
		if (center.z > CHUNK_MAXSIZE) {
			continue;
		}

		// If it is outside the screen, don't bother drawing it
		if (fabsf(center.x) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)
		 || fabsf(center.y) > 1 + fabsf(CHUNK_HEIGHT * 2 / center.w)) {
			// FIXME: Disable this test; one that considers all eight corners of the chunk is needed instead.
			//continue;
		}

		// If this chunk is not initialized, skip it and request meshing
		if(!it.second->isReadyForMeshing()) {
			m_onMeshingRequested(d, gk::Vector3i{it.second->x(), it.second->y(), it.second->z()});

			continue;
		}

		chunksToDraw.emplace_back(it.second.get(), tf);
	}

	drawChunks(target, states, chunksToDraw, currentSky);

	// Restore the camera to its original position
	camera.setDPosition(cameraPos);
}

void ChunkRenderer::drawChunks(gk::RenderTarget &target, gk::RenderStates states, const std::vector<std::pair<ClientChunk*, gk::Transform>> &chunks, const Sky *currentSky) const {
	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	glCheck(glEnable(GL_DEPTH_TEST));

	states.texture = &m_textureAtlas.texture();
	target.beginSceneDraw(states);

	states.shader->setUniform("u_renderDistance", Config::renderDistance * CHUNK_WIDTH);

	if (currentSky)
		states.shader->setUniform("u_fogColor", currentSky->fogColor());

	for (u8 layer = 0 ; layer < ChunkMeshLayer::Count ; ++layer) {
		// Disable mipmaps for specific layers
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,
			(layer == ChunkMeshLayer::NoMipMap || layer == ChunkMeshLayer::Flora) ? 0 : Config::mipmapLevels));

		if (layer == ChunkMeshLayer::Flora || layer == ChunkMeshLayer::Liquid)
			glCheck(glDisable(GL_CULL_FACE));
		else
			glCheck(glEnable(GL_CULL_FACE));

		for (auto &it : chunks) {
			std::size_t verticesCount = it.first->getVerticesCount(layer);
			if (verticesCount == 0) continue;

			states.shader->setUniform("u_modelMatrix", it.second);

			target.drawVertexBuffer(it.first->getVBO(layer), GL_TRIANGLES, 0, (GLsizei)verticesCount, states);

			it.first->setHasBeenDrawn(true);
		}
	}

	target.endSceneDraw(states);

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

