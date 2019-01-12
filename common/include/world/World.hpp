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
// #include "TerrainGenerator.hpp"

class World : public gk::IDrawable {
	public:
		World();

		void update(Player &player);

		Chunk *getChunk(int cx, int cy, int cz) const;

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id);

		u16 getData(int x, int y, int z) const;
		void setData(int x, int y, int z, u16 id);

		BlockData *getBlockData(int x, int y, int z);

		Player *getPlayer() const { return m_player; }

		// Render distance in chunks
		static u16 renderDistance;
		static bool isReloadRequested;

		static World &getInstance() { return *s_instance; }
		static void setInstance(World &instance) { s_instance = &instance; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		// FIXME: Needed for LuaGUIState
		static World *s_instance;
		Player *m_player = nullptr;

		const s32 m_width = 32;
		const s32 m_height = 4;
		const s32 m_depth = 32;

		gk::Texture &m_texture;

		std::vector<std::unique_ptr<Chunk>> m_chunks;

		// TerrainGenerator m_terrainGenerator;
};

#endif // WORLD_HPP_
