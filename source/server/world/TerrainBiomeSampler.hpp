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

#include <FastNoiseLite.hpp>

#include "common/core/IntTypes.hpp"

class Dimension;

class TerrainBiomeSampler {
	public:
		TerrainBiomeSampler(const Dimension &dimension, s32 seed);

		u16 getBiomeIndexAt(s32 x, s32 y);

		// std::vector<WeightedIndex> getWeightedBiomeIndicesAt(double x, double y);

	private:
		// TODO: If kept, should be defined in the worldtype, dynamically
		static const u8 biomeParamCount = 2;

		const Dimension &m_dimension;

		std::vector<FastNoiseLite> m_paramNoises;
};

#endif // TERRAINBIOMESAMPLER_HPP_
