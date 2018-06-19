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
#include "IDrawable.hpp"
#include "NonCopyable.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Chunk : public NonCopyable, public IDrawable {
	public:
		Chunk(s32 x, s32 y, s32 z, Texture &texture);

		void update();

		Block *getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u32 id);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		// const std::vector<std::unique_ptr<Block>> &data() const { return m_data; }

		Chunk *left()  const { return m_surroundingChunks[0]; }
		Chunk *right() const { return m_surroundingChunks[1]; }
		Chunk *front() const { return m_surroundingChunks[2]; }
		Chunk *back()  const { return m_surroundingChunks[3]; }
		Chunk *below() const { return m_surroundingChunks[4]; }
		Chunk *above() const { return m_surroundingChunks[5]; }
		Chunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }

		static const u8 width = 16;
		static const u8 height = 32;
		static const u8 depth = 16;

		void setLeft(Chunk *left)   { m_surroundingChunks[0] = left; }
		void setRight(Chunk *right) { m_surroundingChunks[1] = right; }
		void setFront(Chunk *front) { m_surroundingChunks[2] = front; }
		void setBack(Chunk *back)   { m_surroundingChunks[3] = back; }
		void setBelow(Chunk *below) { m_surroundingChunks[4] = below; }
		void setAbove(Chunk *above) { m_surroundingChunks[5] = above; }

		bool isGenerated() const { return m_isGenerated; }
		bool isInitialized() const { return m_isInitialized; }

		void setChanged(bool isChanged) { m_isChanged = isChanged; }
		void setGenerated(bool isGenerated) { m_isGenerated = isGenerated; }
		void setInitialized(bool isInitialized) { m_isInitialized = isInitialized; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;
		void drawOutlines(RenderTarget &target, RenderStates states) const;

		s32 m_x;
		s32 m_y;
		s32 m_z;

		Texture m_texture;

		// std::vector<std::unique_ptr<Block>> m_data;
		using DataArray = std::array<std::array<std::array<std::unique_ptr<Block>, Chunk::depth>, Chunk::height>, Chunk::width>;
		DataArray m_data;

		std::size_t m_verticesCount;
		std::size_t m_normalsCount;

		VertexBuffer m_vbo{GL_QUADS, 0, 0};

		Chunk *m_surroundingChunks[6];

		bool m_isChanged;
		bool m_isInitialized;
		bool m_isGenerated;
};

#endif // CHUNK_HPP_
