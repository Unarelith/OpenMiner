/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "TerrainBiomeSampler.hpp"
#include "Registry.hpp"
#include "Biome.hpp"

TerrainBiomeSampler::TerrainBiomeSampler() {

	paramNoisesPtr = new FastNoise[nBiomeParams];
	for (u8 i = 0; i < nBiomeParams; i++) {
		paramNoisesPtr[i].SetNoiseType(FastNoise::NoiseType::SimplexFractal);
		paramNoisesPtr[i].SetFrequency(1 / 800.0f);
		paramNoisesPtr[i].SetFractalOctaves(5);
		paramNoisesPtr[i].SetSeed(i);
	}

}

TerrainBiomeSampler::~TerrainBiomeSampler() {
	delete paramNoisesPtr;
}

u16 TerrainBiomeSampler::getBiomeIndexAt(s32 x, s32 y) const {

	// Compute noise instances
	double* biomeParamsPtr = new double[nBiomeParams];
	for (u8 i = 0; i < nBiomeParams; i++) {
		biomeParamsPtr[i] = paramNoisesPtr[i].GetNoise(x, y);
	}

	// TODO with a lot of biomes, perhaps we want an R-Tree or similar, instead of a long loop.
	// Should also finish solving for analytic blending, or find completely separate solution such as isotropically-modified genlayer
	// If we continue with temp/precip/etc params, need to write a weighted lloyd smoother so biomes becone fairly represented.
	// True temp/precip values can then be re-interpolated out from the Voronoi diagram using a neighborhood figure "kernel".
	// TODO with multiple worldtypes added, need to only consider biomes in one worldtype.
	u16 decidedBiomeIndex = 0;
	double decidedBiomeDeviation = 0xFFFF;
	u16 j = 0;
	for (auto &biome : Registry::getInstance().biomes()) {
		double deviation = 0;
		for (int i = 0; i < nBiomeParams; i++) {
			double dp = biomeParamsPtr[i] - biome.get()->getParams()[i];
			deviation += dp * dp;
		}
		if (deviation < decidedBiomeDeviation) {
			decidedBiomeDeviation = deviation;
			decidedBiomeIndex = j;
		}
		j++;
	}

	delete biomeParamsPtr;
	return decidedBiomeIndex;
}