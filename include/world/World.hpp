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
#include "IDrawable.hpp"
#include "TerrainGenerator.hpp"

class Camera;

class World : public IDrawable {
	public:
		World();

		void updateChunks();

		Chunk *getChunk(int cx, int cy, int cz) const;

		u32 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u32 id);

		// Render distance in chunks
		static const u16 renderDistance = 8;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		const s32 m_width = 32;
		const s32 m_height = 4;
		const s32 m_depth = 32;

		Texture &m_texture;

		std::vector<std::unique_ptr<Chunk>> m_chunks;

		TerrainGenerator m_terrainGenerator;
};

#endif // WORLD_HPP_
