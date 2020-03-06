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
#ifndef CHUNKBUILDER_HPP_
#define CHUNKBUILDER_HPP_

#include <array>
#include <vector>

#include <glm/matrix.hpp>

#include <gk/core/Vector3.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Block;
class ClientChunk;
class TextureAtlas;

class ChunkBuilder {
	public:
		ChunkBuilder(TextureAtlas &textureAtlas);

		static constexpr u8 layers = 3;

		std::array<std::size_t, layers> buildChunk(const ClientChunk &chunk, const std::array<gk::VertexBuffer, layers> &vbo);

		enum Layer {
			Solid,
			Liquid,
			Flora,
		};

	private:
		void addFace(s8f x, s8f y, s8f z, s8f i, const ClientChunk &chunk, const Block *block);
		void addCross(s8f x, s8f y, s8f z, const ClientChunk &chunk, const Block *block);

		gk::Vector3i getOffsetFromVertex(u8f i, u8f j) const;
		u8 getAmbientOcclusion(s8f x, s8f y, s8f z, s8f f, s8f v, const ClientChunk &chunk);

		enum class Light {
			Sun,
			Torch
		};

		u8 getLightForVertex(Light light, s8f x, s8f y, s8f z, s8f f, s8f v, const gk::Vector3i &normal, const ClientChunk &chunk);
		void initializeOrientation();

		std::array<std::vector<gk::Vertex>, layers> m_vertices;

		TextureAtlas &m_textureAtlas;
};

#endif // CHUNKBUILDER_HPP_
