/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CHUNKLIGHTMAP_HPP_
#define CHUNKLIGHTMAP_HPP_

#include <queue>

#include <gk/core/IntTypes.hpp>

#include "EngineConfig.hpp"

struct LightNode {
	LightNode(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	int x;
	int y;
	int z;
};

struct LightRemovalNode {
	LightRemovalNode(int _x, int _y, int _z, int _value) : x(_x), y(_y), z(_z), value(_value) {}
	int x;
	int y;
	int z;
	int value;
};

class Chunk;

class ChunkLightmap {
	public:
		ChunkLightmap(Chunk *chunk);

		void addTorchlight(int x, int y, int z, int val);
		void addSunlight(int x, int y, int z, int val);
		void removeTorchlight(int x, int y, int z);
		void removeSunlight(int x, int y, int z);

		bool updateLights();
		bool updateTorchlight();
		bool updateSunlight();

		u8 getLightData(int x, int y, int z) const { return m_lightMap[x][y][z]; }
		u8 getSunlight(int x, int y, int z) const;
		u8 getTorchlight(int x, int y, int z) const;

		void setLightData(int x, int y, int z, u8 val);
		void setSunlight(int x, int y, int z, u8 val);

	private:
		void setTorchlight(int x, int y, int z, u8 val);

		void updateSurroundingChunks(int x, int y, int z);

		Chunk *m_chunk = nullptr;

		using LightMapArray = u8[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		LightMapArray m_lightMap;

		std::queue<LightNode> m_torchlightBfsQueue;
		std::queue<LightNode> m_sunlightBfsQueue;
		std::queue<LightRemovalNode> m_torchlightRemovalBfsQueue;
		std::queue<LightRemovalNode> m_sunlightRemovalBfsQueue;
};

#endif // CHUNKLIGHTMAP_HPP_
