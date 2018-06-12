/*
 * =====================================================================================
 *
 *       Filename:  Chunk.hpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 17:31:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <map>
#include <memory>
#include <vector>

#include "Block.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Chunk {
	public:
		Chunk(s32 x, s32 y, s32 z, Texture &texture);

		void generate();
		void update();

		void draw(Shader &shader);

		Block *getBlock(s8 x, s8 y, s8 z);

		u32 getCoordID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate);

		u32 getVertexID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate);
		u32 getTexCoordID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate);

		bool vertexExists(u8 x, u8 y, u8 z, u8 i, u8 j);

		static float noise2d(float x, float y, int seed, int octaves, float persistence);
		static float noise3d_abs(float x, float y, float z, int seed, int octaves, float persistence);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		bool initialized() const { return m_initialized; }
		void setInitialized(bool initialized) { m_initialized = initialized; }

		Chunk *left()  const { return m_surroundingChunks[0]; }
		Chunk *right() const { return m_surroundingChunks[1]; }
		Chunk *front() const { return m_surroundingChunks[2]; }
		Chunk *back()  const { return m_surroundingChunks[3]; }

		static const u8 width = 16;
		static const u8 height = 32;
		static const u8 depth = 16;

		void setLeft(Chunk *left)   { m_surroundingChunks[0] = left; }
		void setRight(Chunk *right) { m_surroundingChunks[1] = right; }
		void setFront(Chunk *front) { m_surroundingChunks[2] = front; }
		void setBack(Chunk *back)   { m_surroundingChunks[3] = back; }

	private:
		s32 m_x;
		s32 m_y;
		s32 m_z;

		Texture m_texture;

		std::vector<std::unique_ptr<Block>> m_data;

		std::vector<float> m_vertices;
		std::vector<float> m_normals;
		std::vector<float> m_texCoords;

		std::map<u32, u32> m_verticesID;
		std::map<u32, u32> m_extendedFaces;

		VertexBuffer m_vbo;

		Chunk *m_surroundingChunks[4];

		bool m_changed;
		bool m_initialized;
		bool m_generated;
};

#endif // CHUNK_HPP_
