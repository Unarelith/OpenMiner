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

#include <gk/gl/Drawable.hpp>
#include <gk/gl/Texture.hpp>

#include "Chunk.hpp"
#include "ChunkBuilder.hpp"
#include "Config.hpp"

class ClientChunk : public Chunk {
	public:
		ClientChunk(s32 x, s32 y, s32 z, gk::Texture &texture)
			: Chunk(x, y, z), m_texture(texture) {}

		void update();

		void drawLayer(gk::RenderTarget &target, gk::RenderStates states, u8 layer) const;

		bool hasBeenRequested() const { return m_hasBeenRequested; }
		void setHasBeenRequested(bool hasBeenRequested) { m_hasBeenRequested = hasBeenRequested; }

		bool isTooFar() const { return m_isTooFar; }
		void setTooFar(bool isTooFar) { m_isTooFar = isTooFar; }

		bool areAllNeighboursTooFar() const;

	private:
		gk::Texture &m_texture;

		ChunkBuilder m_builder;

		std::array<gk::VertexBuffer, ChunkBuilder::layers> m_vbo{};
		std::array<std::size_t, ChunkBuilder::layers> m_verticesCount{};

		bool m_hasBeenRequested = false;
		bool m_isTooFar = false;
};

#endif // CLIENTCHUNK_HPP_
