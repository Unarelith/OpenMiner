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

#include "ClientChunk.hpp"
#include "TextureAtlas.hpp"

void ClientChunk::update() {
	if (m_lightmap.updateLights() || m_hasChanged || m_hasLightChanged) {
		m_hasChanged = false;
		m_hasLightChanged = false;

		m_verticesCount = m_builder.buildChunk(*this, m_vbo);
	}
}

void ClientChunk::drawLayer(gk::RenderTarget &target, gk::RenderStates states, u8 layer) const {
	if (m_verticesCount.size() <= layer || m_verticesCount.at(layer) == 0) return;

	if (Config::isBloomEffectEnabled) {
		static sf::Texture texture;
		if (!texture.getNativeHandle()) {
			texture.loadFromFile("test_atlas_bloom.png");
		}

		glActiveTexture(GL_TEXTURE1);
		sf::Texture::bind(&texture);
		glActiveTexture(GL_TEXTURE0);

		gk::Shader::bind(states.shader);
		states.shader->setUniform("u_tex", 0);
		states.shader->setUniform("u_bloomTex", 1);
		states.shader->setUniform("u_isBloomEnabled", true);
		gk::Shader::bind(nullptr);
	}
	else {
		gk::Shader::bind(states.shader);
		states.shader->setUniform("u_tex", 0);
		states.shader->setUniform("u_isBloomEnabled", false);
		gk::Shader::bind(nullptr);
	}

	states.texture = &m_textureAtlas.texture();

	if (layer == ChunkBuilder::Layer::Flora || (layer == ChunkBuilder::Layer::Liquid && areAllNeighboursInitialized()))
		glCheck(glDisable(GL_CULL_FACE));
	else
		glCheck(glEnable(GL_CULL_FACE));

	sf::Texture::bind(states.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (layer == ChunkBuilder::Layer::NoMipMap || layer == ChunkBuilder::Layer::Flora) ? 0 : Config::mipmapLevels);
	sf::Texture::bind(nullptr);

	glCheck(glEnable(GL_DEPTH_TEST));

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	target.draw(m_vbo.at(layer), GL_TRIANGLES, 0, m_verticesCount.at(layer), states);
	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

bool ClientChunk::areAllNeighboursTooFar() const {
	return (!m_surroundingChunks[Chunk::West]   || ((ClientChunk *)m_surroundingChunks[Chunk::West])->isTooFar())
		&& (!m_surroundingChunks[Chunk::East]   || ((ClientChunk *)m_surroundingChunks[Chunk::East])->isTooFar())
		&& (!m_surroundingChunks[Chunk::South]  || ((ClientChunk *)m_surroundingChunks[Chunk::South])->isTooFar())
		&& (!m_surroundingChunks[Chunk::North]  || ((ClientChunk *)m_surroundingChunks[Chunk::North])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Bottom] || ((ClientChunk *)m_surroundingChunks[Chunk::Bottom])->isTooFar())
		&& (!m_surroundingChunks[Chunk::Top]    || ((ClientChunk *)m_surroundingChunks[Chunk::Top])->isTooFar());
}

