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
#ifndef TERRAINBIOMESAMPLER_HPP_
#define TERRAINBIOMESAMPLER_HPP_

#include <vector>

#include <gk/core/IntTypes.hpp>

#include "Biome.hpp"
#include "FastNoise.hpp"

class TerrainBiomeSampler {
	public:
		TerrainBiomeSampler(); // TODO should eventually take a worldtype

		u16 getBiomeIndexAt(s32 x, s32 y) const;
		//std::vector<WeightedIndex> getWeightedBiomeIndicesAt(double x, double y);

	private:
		static const u8 biomeParamCount = 2; // TODO if kept, should be defined in the worldtype, dynamically.
		std::vector<FastNoise> m_paramNoises;
};

#endif // TERRAINBIOMESAMPLER_HPP_
