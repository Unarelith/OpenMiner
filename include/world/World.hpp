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

		Chunk *getChunk(int cx, int cz) const;

		Block *getBlock(int x, int y, int z);
		void setBlock(int x, int y, int z, u32 id);

		// Render distance in chunks
		static const u16 renderDistance = 8;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		const s32 m_width = 20;
		const s32 m_height = 1; // FIXME: Never used
		const s32 m_depth = 20;

		Texture m_texture;

		std::vector<std::unique_ptr<Chunk>> m_chunks;

		TerrainGenerator m_terrainGenerator;
};

#endif // WORLD_HPP_
