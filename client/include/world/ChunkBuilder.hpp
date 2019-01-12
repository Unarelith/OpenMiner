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

#include <glm/matrix.hpp>

#include <gk/core/Vector3.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Block;
class ClientChunk;

class ChunkBuilder {
	public:
		static constexpr u8 layers = 3;

		std::array<std::size_t, layers> buildChunk(const ClientChunk &chunk, const std::array<gk::VertexBuffer, layers> &vbo);

		enum Layer {
			Solid,
			Liquid,
			Other,
		};

	private:
		void addFace(u8 x, u8 y, u8 z, u8 i, const ClientChunk &chunk, const Block *block, const Block *surroundingBlock);
		void addCross(u8 x, u8 y, u8 z, const ClientChunk &chunk, const Block *block);

		gk::Vector3i getOffsetFromVertex(u8 i, u8 j);
		u8 getAmbientOcclusion(u8 x, u8 y, u8 z, u8 i, u8 j, const ClientChunk &chunk);

		enum class Light {
			Sun,
			Torch
		};

		float getLightForVertex(Light light, u8 x, u8 y, u8 z, u8 i, u8 j, const ClientChunk &chunk);

		std::array<std::vector<gk::Vertex>, layers> m_vertices;

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
