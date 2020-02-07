/*
 * =====================================================================================
 *
 *       Filename:  World.hpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 15:28:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <memory>

#include "Chunk.hpp"

class World {
	public:
		virtual ~World() = default;

		virtual Chunk *getChunk(int cx, int cy, int cz) const = 0;
		Chunk *getChunkAtBlockPos(int x, int y, int z) const;

		BlockData *getBlockData(int x, int y, int z) const;
		BlockData *addBlockData(int x, int y, int z, int inventoryWidth = 0, int inventoryHeight = 0) const;

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id) const;
		u16 getData(int x, int y, int z) const;
		void setData(int x, int y, int z, u16 data) const;

		static bool isReloadRequested;
};

#endif // WORLD_HPP_
