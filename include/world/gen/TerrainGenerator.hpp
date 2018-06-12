/*
 * =====================================================================================
 *
 *       Filename:  TerrainGenerator.hpp
 *
 *    Description:
 *
 *        Created:  12/06/2018 22:35:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TERRAINGENERATOR_HPP_
#define TERRAINGENERATOR_HPP_

class Chunk;

class TerrainGenerator {
	public:
		void generate(Chunk &chunk);

		static float noise2d(float x, float y, int octaves, float persistence);
		static float noise3d_abs(float x, float y, float z, int octaves, float persistence);
};

#endif // TERRAINGENERATOR_HPP_
