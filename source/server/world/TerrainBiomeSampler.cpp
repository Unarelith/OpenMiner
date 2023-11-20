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
#include "common/core/Registry.hpp"
#include "common/world/Biome.hpp"
#include "server/world/ServerWorld.hpp"
#include "server/world/TerrainBiomeSampler.hpp"

TerrainBiomeSampler::TerrainBiomeSampler(const Dimension &dimension, s32 seed) : m_dimension(dimension) {
	for (u8 i = 0; i < biomeParamCount; i++) {
		m_paramNoises.emplace_back();

		m_paramNoises.back().SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		m_paramNoises.back().SetFractalType(FastNoiseLite::FractalType_FBm);
		m_paramNoises.back().SetFractalOctaves(5);
		m_paramNoises.back().SetFrequency(1 / 800.0f);
		m_paramNoises.back().SetSeed(seed + i);
	}
}

u16 TerrainBiomeSampler::getBiomeIndexAt(s32 x, s32 y) {
	// TODO with a lot of biomes, perhaps we want an R-Tree or similar, instead of a long loop.
	// Should also finish solving for analytic blending, or find completely separate solution such as isotropically-modified genlayer
	// If we continue with temp/precip/etc params, need to write a weighted lloyd smoother so biomes becone fairly represented.
	// True temp/precip values can then be re-interpolated out from the Voronoi diagram using a neighborhood figure "kernel".

	u16 decidedBiomeIndex = 0;
	double decidedBiomeDeviation = INFINITY;

	for (auto &biomeID : m_dimension.biomes()) {
		const Biome &biome = Registry::getInstance().getBiomeFromStringID(biomeID);

		double deviation = 0;
		for (int i = 0; i < biomeParamCount; i++) {
			double dp = m_paramNoises[i].GetNoise<double>(x, y) - biome.getParams()[i];
			deviation += dp * dp;
		}

		if (deviation < decidedBiomeDeviation) {
			decidedBiomeDeviation = deviation;
			decidedBiomeIndex = biome.id();
		}
	}

	return decidedBiomeIndex;
}
