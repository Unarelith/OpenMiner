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

#include <utility>

class Chunk;
class VertexBuffer;

class ChunkBuilder {
	public:
		std::pair<std::size_t, std::size_t> buildChunk(const Chunk &chunk, const VertexBuffer &vbo);
};

#endif // CHUNKBUILDER_HPP_
