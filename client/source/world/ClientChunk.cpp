/*
 * =====================================================================================
 *
 *       Filename:  ClientChunk.cpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 01:32:23
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/gl/GLCheck.hpp>

#include "ClientChunk.hpp"
#include "TextureAtlas.hpp"

void ClientChunk::update() {
	if (m_lightmap.updateLights() || m_hasChanged) {
		m_hasChanged = false;

		m_verticesCount = m_builder.buildChunk(*this, m_vbo);
	}
}

void ClientChunk::drawLayer(gk::RenderTarget &target, gk::RenderStates states, u8 layer) const {
	if (m_verticesCount.size() <= layer || m_verticesCount.at(layer) == 0) return;

	states.texture = &m_textureAtlas.texture();

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_DEPTH_TEST));

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	target.draw(m_vbo.at(layer), GL_TRIANGLES, 0, m_verticesCount.at(layer), states);
	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

bool ClientChunk::areAllNeighboursTooFar() const {
	return (!m_surroundingChunks[Chunk::Left]   || ((ClientChunk *)m_surroundingChunks[Chunk::Left])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Right]  || ((ClientChunk *)m_surroundingChunks[Chunk::Right])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Front]  || ((ClientChunk *)m_surroundingChunks[Chunk::Front])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Back]   || ((ClientChunk *)m_surroundingChunks[Chunk::Back])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Bottom] || ((ClientChunk *)m_surroundingChunks[Chunk::Bottom])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Top]    || ((ClientChunk *)m_surroundingChunks[Chunk::Top])->isTooFar());
}

