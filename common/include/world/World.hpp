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

#include <gk/gl/IDrawable.hpp>

#include "Chunk.hpp"

class World : public gk::IDrawable {
	public:
		void update();

		BlockData *getBlockData(int x, int y, int z);

		// Render distance in chunks
		static u16 renderDistance;
		static bool isReloadRequested;

	private:
		const s32 m_width = 32;
		const s32 m_height = 4;
		const s32 m_depth = 32;
};

#endif // WORLD_HPP_
