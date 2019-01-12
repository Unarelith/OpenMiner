/*
 * =====================================================================================
 *
 *       Filename:  ClientChunk.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 01:31:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CLIENTCHUNK_HPP_
#define CLIENTCHUNK_HPP_

#include <gk/gl/IDrawable.hpp>

#include "ChunkBuilder.hpp"
#include "ChunkLightmap.hpp"
#include "Config.hpp"

class ClientChunk : public gk::IDrawable {
	public:
		enum {
			Left,
			Right,
			Front,
			Back,
			Bottom,
			Top
		};

	public:
		ClientChunk(s32 x, s32 y, s32 z, gk::Texture &texture);

		void update();

		u16 getBlock(int x, int y, int z) const;
		u16 getData(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 type);
		void setData(int x, int y, int z, u16 data);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		ClientChunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		const ClientChunk *getSurroundingChunk(u8 i) const { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		void setSurroundingChunk(u8 i, ClientChunk *chunk) { if (i < 6) m_surroundingChunks[i] = chunk; }

		void setChanged(bool hasChanged) { m_hasChanged = hasChanged; }

		ChunkLightmap &lightmap() { return m_lightmap; }
		const ChunkLightmap &lightmap() const { return m_lightmap; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		s32 m_x;
		s32 m_y;
		s32 m_z;

		gk::Texture &m_texture;

		using DataArray = u32[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		DataArray m_data;

		ChunkBuilder m_builder;
		ChunkLightmap m_lightmap{this};

		std::array<gk::VertexBuffer, ChunkBuilder::layers> m_vbo;
		std::array<std::size_t, ChunkBuilder::layers> m_verticesCount;

		ClientChunk *m_surroundingChunks[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

		bool m_hasChanged = false;
};

#endif // CLIENTCHUNK_HPP_
