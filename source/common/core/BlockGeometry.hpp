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
#ifndef BLOCKGEOMETRY_HPP_
#define BLOCKGEOMETRY_HPP_

#include <gk/core/IntTypes.hpp>
#include <glm/matrix.hpp>

namespace BlockGeometry {

constexpr u8 nAxes = 6;
constexpr u8 nAxesP2 = 8; // nAxes rounded up to the next power of 2
constexpr u8 nRots = 4;
constexpr u8 nFaces = 6;
constexpr u8 nCrossFaces = 2;
constexpr u8 nVertsPerFace = 4;
constexpr u8 nVertsPerCube = 8;
constexpr u8 nCoordsPerUV = 2;
constexpr u8 nOrientations = nAxesP2 * nRots;

// Same order as enum BlockFace in TilesDef.hpp
constexpr u8f cubeVerts[nFaces][nVertsPerFace]{
	// Vertex numbers are encoded according to their binary digits,
	// where bit 0 is X, bit 1 is Y and bit 2 is Z.
	// ZYX    ZYX    ZYX    ZYX
	{0b010, 0b000, 0b100, 0b110},  // West
	{0b001, 0b011, 0b111, 0b101},  // East
	{0b000, 0b001, 0b101, 0b100},  // South
	{0b011, 0b010, 0b110, 0b111},  // North
	{0b010, 0b011, 0b001, 0b000},  // Bottom
	{0b100, 0b101, 0b111, 0b110},  // Top
};

constexpr u8f crossVerts[nCrossFaces][nVertsPerFace]{
	// ZYX    ZYX    ZYX    ZYX
	{0b011, 0b000, 0b100, 0b111},  // NE/SW
	{0b001, 0b010, 0b110, 0b101},  // NW/SE
};

// Normal of each face
// Same order as enum BlockFace in TilesDef.hpp
const glm::vec3 faceNormals[nFaces]{
	{-1, 0, 0},  // West
	{ 1, 0, 0},  // East
	{ 0,-1, 0},  // South
	{ 0, 1, 0},  // North
	{ 0, 0,-1},  // Bottom
	{ 0, 0, 1},  // Top
};

extern glm::mat3 orientMatrices[nOrientations];

void initOrientation(void);

} // namespace BlockGeometry

#endif // BLOCKGEOMETRY_HPP_
