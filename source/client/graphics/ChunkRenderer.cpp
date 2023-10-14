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
#include "RenderTarget.hpp"

inline static bool bbIntersects(const glm::vec3 &a0, const glm::vec3 &a1, const glm::vec3 &b0, const glm::vec3 &b1) {
	return (std::max(a0.x, b0.x) <= std::min(a1.x, b1.x))
	     & (std::max(a0.y, b0.y) <= std::min(a1.y, b1.y))
	     & (std::max(a0.z, b0.z) <= std::min(a1.z, b1.z));
}

inline static bool isOutside(const glm::vec3 &v, const glm::vec3 &n) {
	// Return whether a vertex is on the positive side of the plane given by
	// position (0, 0, 0) and normal n.
	return glm::dot(v, n) >= 0.f;
}

static bool fullyOutside(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &n) {
	// A polyhedron is fully on one side of the axis iff all of its vertices
	// are. This function tests whether all 8 vertices of an axis-aligned box
	// given by the two corners v1, v2 are on the positive side of the plane
	// with normal n that goes through the origin.
	return isOutside(v1, n)
		&& isOutside(v2, n)
		&& isOutside(glm::vec3{v2.x, v1.y, v1.z}, n)
		&& isOutside(glm::vec3{v1.x, v2.y, v2.z}, n)
		&& isOutside(glm::vec3{v1.x, v2.y, v1.z}, n)
		&& isOutside(glm::vec3{v2.x, v1.y, v2.z}, n)
		&& isOutside(glm::vec3{v2.x, v2.y, v1.z}, n)
		&& isOutside(glm::vec3{v1.x, v1.y, v2.z}, n);
}

void ChunkRenderer::draw(RenderTarget &target, RenderStates states, const ChunkMap &chunks, gk::Camera &camera, const Sky *currentSky) const {
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

	const gk::Vector3d cameraPos{camera.getDPosition()};
	camera.setDPosition(0, 0, 0);  // Temporarily move the camera to the origin

	// Calculate the Z of the normalized device coordinate of the render distance plane
	float farZ;
	{
		glm::vec4 farVec{0.f, 0.f, -float(Config::renderDistance * CHUNK_WIDTH), 1.f};
		farVec = target.getView()->getTransform().getMatrix() * farVec;
		farZ = farVec.z / farVec.w;
	}

	// Pre-calculate product of Projection and View matrices
	const glm::mat4 pv = target.getView()->getTransform().getMatrix()
	                   * target.getView()->getViewTransform().getMatrix();

	glm::vec3 frustumVertex[8]; // Frustum vertices

	// Look-at vector (vector the camera is pointing to). It's the Z vector
	// of the inverse of the view matrix. Since the camera is at the origin,
	// the view matrix does not displace, therefore its inverse is the
	// transpose, possibly scaled. But we don't care if the lookat vector is
	// scaled for this purpose.
	const glm::vec3 lookAt{
		-target.getView()->getViewTransform().getMatrix()[0].z,
		-target.getView()->getViewTransform().getMatrix()[1].z,
		-target.getView()->getViewTransform().getMatrix()[2].z,
	};

	glm::vec3 fbb0, fbb1; // Corners of the frustum's AABB
	{
		// Inverse of the PV, used to find the frustum corners in world space
		const glm::mat4 ipv{glm::inverse(pv)};

		int i = 0;
		// Transform the corners of the NDC cube to camera coords.
		// That's where the frustum corners are. Note that we need the
		// frustum truncated by the render distance plane, so we do it here.
		for (float z = -1.f; z <= 1.f; z += 2.f)
		for (float y = -1.f; y <= 1.f; y += 2.f)
		for (float x = -1.f; x <= 1.f; x += 2.f) {
			// Truncate the frustum with the render distance
			glm::vec4 v{ipv * glm::vec4{x, y, std::min(z, farZ), 1.f}};

			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;

			frustumVertex[i].x = v.x;
			frustumVertex[i].y = v.y;
			frustumVertex[i].z = v.z;
			++i;
		}
		// Find the axes-aligned bounding box of the frustum
		fbb0 = glm::vec3{frustumVertex[0]};
		fbb1 = glm::vec3{frustumVertex[0]};
		for (i = 1; i < 8; ++i) {
			// Bottom-left-front corner
			fbb0.x = std::min(fbb0.x, frustumVertex[i].x);
			fbb0.y = std::min(fbb0.y, frustumVertex[i].y);
			fbb0.z = std::min(fbb0.z, frustumVertex[i].z);

			// Top-right-back corner
			fbb1.x = std::max(fbb1.x, frustumVertex[i].x);
			fbb1.y = std::max(fbb1.y, frustumVertex[i].y);
			fbb1.z = std::max(fbb1.z, frustumVertex[i].z);
		}
	}

	// Prepare a list of chunks to draw
	std::vector<std::pair<ClientChunk*, gk::Transform>> chunksToDraw;
	for(auto &it : chunks) {
		if (!it.second->isInitialized()) continue;

		it.second->setHasBeenDrawn(false);

		it.second->setTooFar(false);

		// Apply the Separating Axis Theorem to determine visibility of each
		// chunk. This theorem states that two convex figures don't intersect
		// if, and only if, there's an axis (a line in the case of 2D, a
		// plane in the case of 3D) such that one figure lies completely on
		// one side of the axis and the other lies completely on the other
		// side.
		//
		// Furthermore, if the figures are polygons (2D) or polyhedrons (3D),
		// one of the faces of one of them should be a separating axis, if
		// there's one. But this face can belong to either of the shapes, so
		// all faces of both shapes need to be checked until either one is
		// found which is a separating axis, or none is (in the latter case,
		// the shapes intersect).

		// The goal here is to check if the frustum intersects the chunk's
		// bounding box, which is an axes-aligned box, and we take advantage
		// of that, because checking all the points of a shape against an
		// axis-aligned box is equivalent to checking intersection between
		// both shapes' axis-aligned bounding boxes. Therefore, our first
		// check will apply the theorem to the sides of the chunk's bounding
		// box, against the frustum's AABB which we have calculated before.
		// This effectively checks if any of the sides of the chunk's BB is a
		// separating axis, in the sense of the theorem.

		// Find bottom-left-front corner of chunk's BB (in cameraPos coords)
		const glm::vec3 chunkBB0{
			it.second->x() * CHUNK_WIDTH  - cameraPos.x,
			it.second->y() * CHUNK_DEPTH  - cameraPos.y,
			it.second->z() * CHUNK_HEIGHT - cameraPos.z,
		};

		// Find top-right-back corner of chunk's BB
		const glm::vec3 chunkBB1{
			chunkBB0.x + CHUNK_WIDTH,
			chunkBB0.y + CHUNK_DEPTH,
			chunkBB0.z + CHUNK_HEIGHT
		};

		// Check if it's too far (temporary hack for removing chunks, until
		// it's performed with a timeout)
		const float dist = glm::length((chunkBB0 + chunkBB1) / 2.f);
		if (dist > glm::length(frustumVertex[4]) + glm::length(glm::vec3{CHUNK_WIDTH, CHUNK_DEPTH, CHUNK_HEIGHT}) / 2.f) {
			it.second->setTooFar(true);

			continue;
		}

		// Check if the frustum's BB intersects the chunk's BB.
		// This discards the big majority of chunks, and the more expensive
		// tests will be done only on a handful on them, of which roughly
		// one third will be visible.
		if (!bbIntersects(chunkBB0, chunkBB1, fbb0, fbb1))
			continue;

		// Now it's the turn to check each of the frustum's faces against the
		// chunk's BB. If none of them is a separating axis, then the frustum
		// and the chunk's BB definitively intersect, i.e. the BB is visible.
		// We'll consider the frustum as a pyramid, i.e. we'll ignore the
		// near clipping plane. This is OK because the worst case is that
		// we'll render up to 4 more chunks than necessary, in the rare
		// event that the corner of the chunks is within the tiny pyramid
		// situated between the near plane and the camera's position. This
		// saves one of the checks.

		// All frustum planes considered except one intersect the camera's
		// origin. For that one (see next check) we subtract the vertex from
		// the BB corners, to make the test correct.

		// The far plane (or more precisely, the render distance plane) uses
		// the lookat vector as the normal. Any vertex on the far side of the
		// clipped frustum will do as position. This check is also used to
		// set the TooFar flag on the chunk.
		if (fullyOutside(chunkBB0 - frustumVertex[4], chunkBB1 - frustumVertex[4], lookAt))
			continue;

		// Check the chunk's BB against the remaining 4 frustum planes.
		// Normals (cross products) must be calculated in right-hand order
		// so that all normals point outwards of the frustum (pyramid).
		// The top and bottom planes discard the most chunks, because the
		// frustum is narrower in the vertical direction, so they are checked
		// first.
		if (fullyOutside(chunkBB0, chunkBB1, glm::cross(frustumVertex[4], frustumVertex[5]))
		 || fullyOutside(chunkBB0, chunkBB1, glm::cross(frustumVertex[7], frustumVertex[6]))
		 || fullyOutside(chunkBB0, chunkBB1, glm::cross(frustumVertex[5], frustumVertex[7]))
		 || fullyOutside(chunkBB0, chunkBB1, glm::cross(frustumVertex[6], frustumVertex[4]))
		)
			continue;

		// If this chunk is not initialized, skip it and request meshing
		if(!it.second->isReadyForMeshing()) {
			m_onMeshingRequested(dist, gk::Vector3i{it.second->x(), it.second->y(), it.second->z()});

			continue;
		}

		// The chunk's model matrix is a pure translation matrix whose origin
		// is the chunk's origin in cameraPos coordinates.
		const glm::mat4 tf{1.f, 0.f, 0.f, 0.f,
		                   0.f, 1.f, 0.f, 0.f,
		                   0.f, 0.f, 1.f, 0.f,
		                   chunkBB0.x, chunkBB0.y, chunkBB0.z, 1.f};

		chunksToDraw.emplace_back(it.second.get(), tf);
	}

	drawChunks(target, states, chunksToDraw, currentSky);

	// Restore the camera to its original position
	camera.setDPosition(cameraPos);
}

void ChunkRenderer::drawChunks(RenderTarget &target, RenderStates states, const std::vector<std::pair<ClientChunk*, gk::Transform>> &chunks, const Sky *currentSky) const {
	++ClientChunk::frameCounter;

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	glCheck(glEnable(GL_DEPTH_TEST));

	states.texture = &m_textureAtlas.texture();

	target.beginDrawing(states);

	states.shader->setUniform("u_renderDistance", Config::renderDistance * CHUNK_WIDTH);

	if (currentSky)
		states.shader->setUniform("u_fogColor", currentSky->fogColor());

	GLint modelMatrixUniform = states.shader->uniform("u_modelMatrix");

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

			states.shader->setUniform(modelMatrixUniform, it.second);

			gk::VertexArray::bind(&it.first->getVertexArray());
			target.drawArrays(GL_TRIANGLES, (GLint)it.first->getBufferOffset(layer), (GLsizei)verticesCount);
			gk::VertexArray::bind(nullptr);

			if (!it.first->hasBeenDrawn())
				++ClientChunk::chunkDrawCounter;

			++ClientChunk::chunkDrawCallCounter;

			it.first->setHasBeenDrawn(true);
		}
	}

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

