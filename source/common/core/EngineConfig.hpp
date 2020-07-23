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
#ifndef ENGINECONFIG_HPP_
#define ENGINECONFIG_HPP_

namespace {
	// Only used in Client
	constexpr const char *APP_NAME = "OpenMiner";

	constexpr unsigned char VERSION_MAJOR = 0;
	constexpr unsigned char VERSION_MINOR = 0;
	constexpr unsigned char VERSION_PATCH = 9;

	constexpr float DIST_NEAR  = 0.1f;
	constexpr float DIST_FAR   = 1000.0f;
	constexpr float DIST_SUN   = 950.0f;
	constexpr float DIST_MOON  = 950.0f;
	constexpr float DIST_STARS = 975.0f;

	constexpr float DIST_2D_NEAR = -512.0f;
	constexpr float DIST_2D_FAR  = 512.0f;

	// Chunk size must be a power of two and fit in a signed byte
	constexpr int CHUNK_WIDTH  = 16;
	constexpr int CHUNK_DEPTH  = 16;
	constexpr int CHUNK_HEIGHT = 32;

	constexpr int CHUNK_MAXSIZE = CHUNK_WIDTH > CHUNK_DEPTH ? (CHUNK_HEIGHT > CHUNK_WIDTH ? CHUNK_HEIGHT : CHUNK_WIDTH) : (CHUNK_HEIGHT > CHUNK_DEPTH ? CHUNK_HEIGHT : CHUNK_DEPTH);

	// Several parts of the code use & (CHUNK_xxx - 1) assuming they are powers of 2
	static_assert((CHUNK_WIDTH  & (CHUNK_WIDTH  - 1)) == 0, "CHUNK_WIDTH is not a power of 2");
	static_assert((CHUNK_DEPTH  & (CHUNK_DEPTH  - 1)) == 0, "CHUNK_DEPTH is not a power of 2");
	static_assert((CHUNK_HEIGHT & (CHUNK_HEIGHT - 1)) == 0, "CHUNK_HEIGHT is not a power of 2");

	// Several parts of the code use s8 for in-chunk coordinates
	static_assert(CHUNK_WIDTH >= -128 && CHUNK_WIDTH < 128, "CHUNK_WIDTH out of range");
	static_assert(CHUNK_HEIGHT >= -128 && CHUNK_HEIGHT < 128, "CHUNK_HEIGHT out of range");
	static_assert(CHUNK_DEPTH >= -128 && CHUNK_DEPTH < 128, "CHUNK_DEPTH out of range");

	constexpr int SEALEVEL = 4;

	constexpr const char *BLOCK_AIR = "_:air";
}

#endif // ENGINECONFIG_HPP_
