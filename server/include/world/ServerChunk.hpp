/*
 * =====================================================================================
 *
 *       Filename:  ServerChunk.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 01:32:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERCHUNK_HPP_
#define SERVERCHUNK_HPP_

#include <gk/core/IntTypes.hpp>

#include "Config.hpp"
#include "TerrainGenerator.hpp"

class ServerChunk {
	using DataArray = u32[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];

	public:
		ServerChunk(s32 x, s32 y, s32 z);

		void generate();

		u16 getBlock(int x, int y, int z) const;
		u16 getData(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 type);
		void setData(int x, int y, int z, u16 data);

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		ServerChunk *getSurroundingChunk(u8 i) { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		const ServerChunk *getSurroundingChunk(u8 i) const { return (i > 5) ? nullptr : m_surroundingChunks[i]; }
		void setSurroundingChunk(u8 i, ServerChunk *chunk) { if (i < 6) m_surroundingChunks[i] = chunk; }

		const DataArray &data() const { return m_data; }

	private:
		DataArray m_data;

		s32 m_x;
		s32 m_y;
		s32 m_z;

		bool m_hasChanged = false;
		bool m_isGenerated = false;

		TerrainGenerator m_terrainGenerator;

		ServerChunk *m_surroundingChunks[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
};

#endif // SERVERCHUNK_HPP_
