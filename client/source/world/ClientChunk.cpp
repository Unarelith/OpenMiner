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
#include "ClientChunk.hpp"

void ClientChunk::update() {
	if (m_isInitialized && m_hasChanged) {
		m_hasChanged = false;
		m_lightmap.updateLights();

		m_verticesCount = m_builder.buildChunk(*this, m_vbo);
	}
}

void ClientChunk::drawLayer(gk::RenderTarget &target, gk::RenderStates states, u8 layer) const {
	if (m_verticesCount.size() <= layer || m_verticesCount.at(layer) == 0) return;

	states.texture = &m_texture;

	if (layer == ChunkBuilder::Layer::Other)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	if(Config::isWireframeModeEnabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	target.draw(m_vbo.at(layer), GL_TRIANGLES, 0, m_verticesCount.at(layer), states);
	if(Config::isWireframeModeEnabled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

