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

#include <gk/core/Vector3.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/gl/VertexBuffer.hpp>
#include <gk/utils/NonCopyable.hpp>

#include "Block.hpp"
#include "BlockData.hpp"
#include "ChunkLightmap.hpp"

class Chunk : public gk::NonCopyable {
	public:
		enum {
			Left,    // cx - 1
			Right,   // cx + 1
			Front,   // cz - 1
			Back,    // cz + 1
			Bottom,  // cy - 1
			Top      // cy + 1
		};

	public:
		Chunk(s32 x, s32 y, s32 z);
		virtual ~Chunk() = default;

		u16 getBlock(int x, int y, int z) const;
		u16 getData(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 type);
		void setData(int x, int y, int z, u16 data);

		void setBlockRaw(int x, int y, int z, u16 block);

		BlockData *getBlockData(int x, int y, int z);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		Chunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		void setSurroundingChunk(u8 i, Chunk *chunk) { if (i < 6) m_surroundingChunks[i] = chunk; }

		bool areAllNeighboursLoaded() const;
		bool areAllNeighboursInitialized() const;

		bool hasChanged() const { return m_hasChanged; }
		void setChanged(bool hasChanged) { m_hasChanged = hasChanged; }

		bool isInitialized() const { return m_isInitialized; }
		void setInitialized(bool isInitialized) { m_isInitialized = isInitialized; }

		ChunkLightmap &lightmap() { return m_lightmap; }
		const ChunkLightmap &lightmap() const { return m_lightmap; }

		static constexpr u8 width = CHUNK_WIDTH;
		static constexpr u8 height = CHUNK_HEIGHT;
		static constexpr u8 depth = CHUNK_DEPTH;

		using DataArray = u32[Chunk::width][Chunk::height][Chunk::depth];
		const DataArray &data() const { return m_data; }

	protected:
		// void updateNeighbours(int x, int y, int z);

		s32 m_x;
		s32 m_y;
		s32 m_z;

		DataArray m_data;

		ChunkLightmap m_lightmap{this};

		Chunk *m_surroundingChunks[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

		bool m_hasChanged = false;
		bool m_isInitialized = false;

		std::unordered_map<std::size_t, const Block&> m_tickingBlocks;

		std::unordered_map<gk::Vector3i, BlockData> m_blockData;
};

#endif // CHUNK_HPP_
