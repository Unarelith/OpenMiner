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
#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

#include <gk/core/Vector3.hpp>
#include <gk/utils/NonCopyable.hpp>

#include "Block.hpp"
#include "BlockData.hpp"
#include "ChunkLightmap.hpp"

class World;

class Chunk : public gk::NonCopyable {
	public:
		enum {
			West,    // cx - 1
			East,    // cx + 1
			South,   // cy - 1
			North,   // cy + 1
			Bottom,  // cz - 1
			Top      // cz + 1
		};

	public:
		Chunk(s32 x, s32 y, s32 z, World &world);
		virtual ~Chunk() = default;

		virtual void update() = 0;
		virtual void process() = 0;

		u32 getFullBlock(int x, int y, int z) const;
		void setDataRaw(u8 x, u8 y, u8 z, u32 data) { m_data[z][y][x] = data; m_hasChanged = true; }

		u16 getBlock(int x, int y, int z) const;
		u16 getData(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 type);
		bool setData(int x, int y, int z, u16 data);

		bool setBlockRaw(int x, int y, int z, u16 block);

		const BlockState *getBlockState(int x, int y, int z) const;
		void setBlockState(int x, int y, int z, u16 stateID);

		virtual void onBlockPlaced(int, int, int, const Block &) {}
		virtual void onBlockDestroyed(int, int, int, const Block &) {}

		BlockData *getBlockData(int x, int y, int z) const;
		BlockData *addBlockData(int x, int y, int z, u16 inventoryWidth = 0, u16 inventoryHeight = 0);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		World &world() { return m_world; }

		bool hasSurroundingChunk(u8 i) { return (i > 5) ? false : m_surroundingChunkPresence[i]; }
		Chunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		const Chunk *getSurroundingChunk(u8 i) const { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		void setSurroundingChunk(u8 i, Chunk *chunk);

		bool areAllNeighboursInitialized() const;
		bool areAllNeighboursLoaded() const { return m_surroundingChunkPresence.all(); }

		bool isInitialized() const { return m_isInitialized; }
		void setInitialized(bool isInitialized) { m_isInitialized = isInitialized; }

		void setChanged() { m_hasChanged = true; }

		ChunkLightmap &lightmap() { return m_lightmap; }
		const ChunkLightmap &lightmap() const { return m_lightmap; }

		static constexpr u8 width = CHUNK_WIDTH;
		static constexpr u8 depth = CHUNK_DEPTH;
		static constexpr u8 height = CHUNK_HEIGHT;

		// Placing horizontal slices together may help with cache locality and compressibility
		using DataArray = u32[Chunk::height][Chunk::depth][Chunk::width];
		const DataArray &data() const { return m_data; }

		u32 data(int x, int y, int z) const { return m_data[z][y][x]; }

		static void initUsertype(sol::state &lua);

	protected:
		s32 m_x;
		s32 m_y;
		s32 m_z;

		World &m_world;

		DataArray m_data;

		ChunkLightmap m_lightmap{this};

		Chunk *m_surroundingChunks[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		std::bitset<6> m_surroundingChunkPresence;

		bool m_isInitialized = false;
		bool m_hasChanged = false;

		std::unordered_map<gk::Vector3i, std::unique_ptr<BlockData>> m_blockData;
};

#endif // CHUNK_HPP_
