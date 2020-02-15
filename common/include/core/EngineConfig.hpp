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
#ifndef ENGINECONFIG_HPP_
#define ENGINECONFIG_HPP_

namespace {
	// FIXME: Only used in Client
	constexpr const char *APP_NAME = "OpenMiner";

	constexpr float DIST_NEAR = 0.1f;
	constexpr float DIST_FAR  = 1000.0f;

	constexpr int CHUNK_WIDTH  = 16;
	constexpr int CHUNK_HEIGHT = 32;
	constexpr int CHUNK_DEPTH  = 16;

	constexpr int SEALEVEL = 4;
}

#endif // ENGINECONFIG_HPP_
