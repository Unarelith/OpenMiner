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

#include <array>
#include <vector>

#include "IntTypes.hpp"
#include "Vertex.hpp"

class Block;
class Chunk;
class VertexBuffer;

class ChunkBuilder {
	public:
		std::pair<std::size_t, std::size_t> buildChunk(const Chunk &chunk, const VertexBuffer &vbo, const VertexBuffer &liquidVbo);

	private:
		void addFace(u8 x, u8 y, u8 z, u8 i, const Chunk &chunk, const Block *block, const Block *surroundingBlock);

		float getAverageTorchlight(u8 x, u8 y, u8 z, s8 offsetX, s8 offsetY, s8 offsetZ, const Chunk &chunk);
		float getAverageSunlight(u8 x, u8 y, u8 z, s8 offsetX, s8 offsetY, s8 offsetZ, const Chunk &chunk);

		float getSunlightForVertex(u8 x, u8 y, u8 z, u8 i, u8 j, const Chunk &chunk);
		float getTorchlightForVertex(u8 x, u8 y, u8 z, u8 i, u8 j, const Chunk &chunk);

		std::array<std::vector<Vertex>, 2> m_vertices;

		enum Face {
			Left,
			Right,
			Bottom,
			Top,
			Front,
			Back
		};
};

#endif // CHUNKBUILDER_HPP_
