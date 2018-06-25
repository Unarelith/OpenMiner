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

#include <memory>
#include <unordered_map>
#include <vector>

#include "Block.hpp"
#include "ChunkBuilder.hpp"
#include "ChunkLightmap.hpp"
#include "IDrawable.hpp"
#include "NonCopyable.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

class Chunk : public NonCopyable, public IDrawable {
	public:
		Chunk(s32 x, s32 y, s32 z, Texture &texture);

		void update(World &world);

		u32 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u32 id);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		Chunk *left()  const { return m_surroundingChunks[0]; }
		Chunk *right() const { return m_surroundingChunks[1]; }
		Chunk *front() const { return m_surroundingChunks[2]; }
		Chunk *back()  const { return m_surroundingChunks[3]; }
		Chunk *below() const { return m_surroundingChunks[4]; }
		Chunk *above() const { return m_surroundingChunks[5]; }
		Chunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		const Chunk *getSurroundingChunk(u8 i) const { return (i > 5) ? nullptr : m_surroundingChunks[i]; }

		static constexpr u8 width = CHUNK_WIDTH;
		static constexpr u8 height = CHUNK_HEIGHT;
		static constexpr u8 depth = CHUNK_DEPTH;

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

		ChunkLightmap &lightmap() { return m_lightmap; }
		const ChunkLightmap &lightmap() const { return m_lightmap; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;
		void drawOutlines(RenderTarget &target, RenderStates states) const;

		s32 m_x;
		s32 m_y;
		s32 m_z;

		Texture &m_texture;

		// using DataArray = std::array<std::array<std::array<std::unique_ptr<Block>, Chunk::depth>, Chunk::height>, Chunk::width>;
		using DataArray = u32[Chunk::width][Chunk::height][Chunk::depth];
		DataArray m_data;

		ChunkBuilder m_builder;
		ChunkLightmap m_lightmap{this};

		VertexBuffer m_vbo;
		std::size_t m_verticesCount;

		Chunk *m_surroundingChunks[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

		bool m_isChanged = false;
		bool m_isInitialized = false;
		bool m_isGenerated = false;

		std::unordered_map<std::size_t, const Block&> m_tickingBlocks;
};

#endif // CHUNK_HPP_
