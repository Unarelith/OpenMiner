/*
 * =====================================================================================
 *
 *       Filename:  ChunkLightmap.hpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 14:32:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CHUNKLIGHTMAP_HPP_
#define CHUNKLIGHTMAP_HPP_

#include <queue>

#include <gk/core/IntTypes.hpp>

#include "Config.hpp"

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

		void addLight(int x, int y, int z, int val);
		void addSunlight(int x, int y, int z, int val);
		void removeLight(int x, int y, int z);
		void removeSunlight(int x, int y, int z);

		void updateLights();
		void updateTorchlight();
		void updateSunlight();

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

		std::queue<LightNode> m_lightBfsQueue;
		std::queue<LightNode> m_sunlightBfsQueue;
		std::queue<LightRemovalNode> m_lightRemovalBfsQueue;
		std::queue<LightRemovalNode> m_sunlightRemovalBfsQueue;
};

#endif // CHUNKLIGHTMAP_HPP_
