/*
 * =====================================================================================
 *
 *       Filename:  ChunkBuilder.hpp
 *
 *    Description:
 *
 *        Created:  19/06/2018 22:26:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CHUNKBUILDER_HPP_
#define CHUNKBUILDER_HPP_

#include <vector>

#include "IntTypes.hpp"
#include "Vertex.hpp"

class Block;
class Chunk;
class VertexBuffer;

class ChunkBuilder {
	public:
		std::size_t buildChunk(const Chunk &chunk, const VertexBuffer &vbo);

	private:
		void addFace(u8 x, u8 y, u8 z, u8 i, const Chunk &chunk, const Block *block, const Block *surroundingBlock);

		std::vector<Vertex> m_vertices;
};

#endif // CHUNKBUILDER_HPP_
