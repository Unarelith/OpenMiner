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
#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <memory>
#include <set>

#include "world/Chunk.hpp"

class World {
	public:
		virtual ~World() = default;

		void update();

		virtual Chunk *getChunk(int cx, int cy, int cz) const = 0;
		Chunk *getChunkAtBlockPos(int x, int y, int z) const;

		BlockData *getBlockData(int x, int y, int z) const;
		BlockData *addBlockData(int x, int y, int z, u16 inventoryWidth = 0, u16 inventoryHeight = 0) const;

		const BlockState *getBlockState(int x, int y, int z) const;
		void setBlockState(int x, int y, int z, u16 stateID);

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id) const;
		u16 getData(int x, int y, int z) const;
		void setData(int x, int y, int z, u16 data) const;

		bool addChunkToUpdate(Chunk *chunk);
		bool addChunkToProcess(Chunk *chunk);

		virtual void onBlockPlaced(int, int, int, const Block &) {}

		static void initUsertype(sol::state &lua);

		static bool isReloadRequested;

	protected:
		void clearUpdateQueues();

	private:
		std::set<Chunk *> m_chunksToUpdate;
		std::queue<Chunk *> m_chunkUpdateQueue;

		std::set<Chunk *> m_chunksToProcess;
		std::queue<Chunk *> m_chunkProcessQueue;
};

#endif // WORLD_HPP_
