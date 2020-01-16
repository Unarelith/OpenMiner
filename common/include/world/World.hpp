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
		BlockData *getBlockData(int x, int y, int z) const;

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id) const;
		u16 getData(int x, int y, int z) const;
		void setData(int x, int y, int z, u16 id) const;

		static bool isReloadRequested;
};

#endif // WORLD_HPP_
